//
//  FSActionSheet.m
//  FSActionSheet
//
//  Created by Steven on 6/7/16.
//  Copyright © 2016年 Steven. All rights reserved.
//

#import "FSActionSheet.h"
#import "FSActionSheetCell.h"

CGFloat const kFSActionSheetSectionHeight = 10; ///< 分区间距

@interface FSActionSheet () <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *cancelTitle;
@property (nonatomic, copy) NSArray<FSActionSheetItem *> *items;
@property (nonatomic, copy) FSActionSheetHandler selectedHandler;

@property (nonatomic, strong) UIWindow *window;
@property (nonatomic, weak)   UIView *controllerView;
@property (nonatomic, strong) UIView *backView;
@property (nonatomic, strong) UITableView *tableView;
@property (nonatomic, weak)   UILabel *titleLabel;

@property (nonatomic, strong) NSLayoutConstraint *heightConstraint; ///< 内容高度约束

@end

static NSString * const kFSActionSheetCellIdentifier = @"kFSActionSheetCellIdentifier";

@implementation FSActionSheet

- (instancetype)init {
    return [self initWithTitle:nil cancelTitle:nil items:nil];
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

/*! @brief 单文本选项快速初始化 */
- (instancetype)initWithTitle:(NSString *)title delegate:(id<FSActionSheetDelegate>)delegate cancelButtonTitle:(NSString *)cancelButtonTitle highlightedButtonTitle:(NSString *)highlightedButtonTitle otherButtonTitles:(NSArray<NSString *> *)otherButtonTitles {
    if (!(self = [super init])) return nil;
    
    [self baseSetting];
    
    NSMutableArray *titleItems = [@[] mutableCopy];
    // 普通按钮
    for (NSString *otherTitle in otherButtonTitles) {
        if (otherTitle && otherTitle.length > 0) {
            [titleItems addObject:FSActionSheetTitleItemMake(FSActionSheetTypeNormal, otherTitle)];
        }
    }
    // 高亮按钮, 高亮按钮放在最下面.
    if (highlightedButtonTitle && highlightedButtonTitle.length > 0) {
        [titleItems addObject:FSActionSheetTitleItemMake(FSActionSheetTypeHighlighted, highlightedButtonTitle)];
    }
    
    self.title = title?:@"";
    self.delegate = delegate;
    self.cancelTitle = (cancelButtonTitle && cancelButtonTitle.length != 0)?cancelButtonTitle:@"取消";
    self.items = titleItems;
    
    [self addSubview:self.tableView];
    
    return self;
}

/*! @brief 在外部组装选项按钮item */
- (instancetype)initWithTitle:(NSString *)title cancelTitle:(NSString *)cancelTitle items:(NSArray<FSActionSheetItem *> *)items {
    if (!(self = [super init])) return nil;
    
    [self baseSetting];
    
    self.title = title?:@"";
    self.cancelTitle = (cancelTitle && cancelTitle.length != 0)?cancelTitle:@"取消";
    self.items = items?:@[];
    
    [self addSubview:self.tableView];
    
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    self.tableView.frame = self.bounds;
}

// 默认设置
- (void)baseSetting {
    self.backgroundColor = FSColorWithString(FSActionSheetBackColor);
    self.translatesAutoresizingMaskIntoConstraints = NO; // 允许约束
    
    _contentAlignment = FSContentAlignmentCenter; // 默认样式为居中
    _hideOnTouchOutside = YES; // 默认点击半透明层隐藏弹窗
    
    // 监听屏幕旋转
    [[NSNotificationCenter defaultCenter] addObserver:self  selector:@selector(orientationDidChange:) name:UIDeviceOrientationDidChangeNotification  object:nil];
}

// 屏幕旋转通知回调
- (void)orientationDidChange:(NSNotification *)notification {
    if (_title.length > 0) {
        // 更新头部标题的高度
        CGFloat newHeaderHeight = [self heightForHeaderView];
        CGRect newHeaderRect = _tableView.tableHeaderView.frame;
        newHeaderRect.size.height = newHeaderHeight;
        _tableView.tableHeaderView.frame = newHeaderRect;
        self.tableView.tableHeaderView = self.tableView.tableHeaderView;
        // 适配当前内容高度
        [self fixContentHeight];
    }
}

#pragma mark - private
// 计算title在设定宽度下的富文本高度
- (CGFloat)heightForHeaderView {
    CGFloat labelHeight = [_titleLabel.attributedText boundingRectWithSize:CGSizeMake(CGRectGetWidth(self.window.frame)-FSActionSheetDefaultMargin*2, MAXFLOAT) options:NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading context:nil].size.height;
    CGFloat headerHeight = ceil(labelHeight)+FSActionSheetDefaultMargin*2;
    
    return headerHeight;
}

// 整个弹窗内容的高度
- (CGFloat)contentHeight {
    CGFloat titleHeight = (_title.length > 0)?[self heightForHeaderView]:0;
    CGFloat rowHeightSum = (_items.count+1)*FSActionSheetRowHeight+kFSActionSheetSectionHeight;
    CGFloat contentHeight = titleHeight+rowHeightSum;
    
    return contentHeight;
}

// 适配屏幕高度, 弹出窗高度不应该高于屏幕的设定比例
- (void)fixContentHeight {
    CGFloat contentMaxHeight = CGRectGetHeight(self.window.frame)*FSActionSheetContentMaxScale;
    CGFloat contentHeight = [self contentHeight];
    if (contentHeight > contentMaxHeight) {
        contentHeight = contentMaxHeight;
        self.tableView.scrollEnabled = YES;
    } else {
        self.tableView.scrollEnabled = NO;
    }
    
    _heightConstraint.constant = contentHeight;
}

// 适配标题偏移方向
- (void)updateTitleAttributeText {
    if (_title.length == 0 || !_titleLabel) return;
    
    // 富文本相关配置
    NSRange  attributeRange = NSMakeRange(0, _title.length);
    UIFont  *titleFont      = [UIFont systemFontOfSize:14];
    UIColor *titleTextColor = FSColorWithString(FSActionSheetTitleColor);
    CGFloat  lineSpacing = FSActionSheetTitleLineSpacing;
    CGFloat  kernSpacing = FSActionSheetTitleKernSpacing;
    
    NSMutableAttributedString *titleAttributeString = [[NSMutableAttributedString alloc] initWithString:_title];
    NSMutableParagraphStyle *titleStyle = [[NSMutableParagraphStyle alloc] init];
    // 行距
    titleStyle.lineSpacing = lineSpacing;
    // 内容偏移样式
    switch (_contentAlignment) {
        case FSContentAlignmentLeft: {
            titleStyle.alignment = NSTextAlignmentLeft;
            break;
        }
        case FSContentAlignmentCenter: {
            titleStyle.alignment = NSTextAlignmentCenter;
            break;
        }
        case FSContentAlignmentRight: {
            titleStyle.alignment = NSTextAlignmentRight;
            break;
        }
    }
    [titleAttributeString addAttribute:NSParagraphStyleAttributeName value:titleStyle range:attributeRange];
    // 字距
    [titleAttributeString addAttribute:NSKernAttributeName value:@(kernSpacing) range:attributeRange];
    // 字体
    [titleAttributeString addAttribute:NSFontAttributeName value:titleFont range:attributeRange];
    // 颜色
    [titleAttributeString addAttribute:NSForegroundColorAttributeName value:titleTextColor range:attributeRange];
    _titleLabel.attributedText = titleAttributeString;
}

// 点击背景半透明遮罩层隐藏
- (void)backViewGesture {
    [self hideWithCompletion:nil];
}
- (void)hidethis
{
    [UIView animateWithDuration:0.25 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
        _backView.alpha   = 0;
        CGRect newFrame   = self.frame;
        newFrame.origin.y = CGRectGetMaxY(_controllerView.frame);
        self.frame        = newFrame;
    } completion:^(BOOL finished) {
        [[[UIApplication sharedApplication].delegate window] makeKeyWindow];
        [_backView removeFromSuperview];
        _backView = nil;
        [_tableView removeFromSuperview];
        _tableView = nil;
        [self removeFromSuperview];
        self.window = nil;
        self.selectedHandler = nil;
    }];
}
// 隐藏
- (void)hideWithCompletion:(void(^)())completion {
    [UIView animateWithDuration:0.25 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
        _backView.alpha   = 0;
        CGRect newFrame   = self.frame;
        newFrame.origin.y = CGRectGetMaxY(_controllerView.frame);
        self.frame        = newFrame;
    } completion:^(BOOL finished) {
        [[[UIApplication sharedApplication].delegate window] makeKeyWindow];
        if (completion) completion();
        [_backView removeFromSuperview];
        _backView = nil;
        [_tableView removeFromSuperview];
        _tableView = nil;
        [self removeFromSuperview];
        self.window = nil;
        self.selectedHandler = nil;
    }];
}

#pragma mark - public
/*! @brief 单展示, 不绑定block回调 */
- (void)show {
    [self showWithSelectedCompletion:NULL];
}

- (CGSize)xl_size
{
    return self.frame.size;
}

- (void)setXl_size:(CGSize)xl_size
{
    CGRect frame = self.frame;
    frame.size = xl_size;
    self.frame = frame;
}

- (CGFloat)xl_width
{
    return self.frame.size.width;
}

- (void)setXl_width:(CGFloat)xl_width
{
    CGRect rect = self.frame;
    rect.size.width = xl_width;
    self.frame = rect;
}

- (CGFloat)xl_height
{
    return self.frame.size.height;
}

- (void)setXl_height:(CGFloat)xl_height
{
    CGRect rect = self.frame;
    rect.size.height = xl_height;
    self.frame = rect;
}

- (CGFloat)xl_x
{
    return self.frame.origin.x;
}

- (void)setXl_x:(CGFloat)xl_x
{
    CGRect rect = self.frame;
    rect.origin.x = xl_x;
    self.frame = rect;
}

- (CGFloat)xl_y
{
    return self.frame.origin.y;
}

- (void)setXl_y:(CGFloat)xl_y
{
    CGRect rect = self.frame;
    rect.origin.y = xl_y;
    self.frame = rect;
}

- (CGFloat)xl_centerX
{
    return self.center.x;
}

- (void)setXl_centerX:(CGFloat)xl_centerX
{
    CGPoint point = self.center;
    point.x = xl_centerX;
    self.center = point;
}

- (CGFloat)xl_centerY
{
    return self.center.y;
}

- (void)setXl_centerY:(CGFloat)xl_centerY
{
    CGPoint point = self.center;
    point.y = xl_centerY;
    self.center = point;
}

- (CGFloat)height
{
    return self.frame.size.height;
}

- (void)setHeight:(CGFloat)height
{
    CGRect rect = self.frame;
    rect.size.height = height;
    self.frame = rect;
}

- (CGFloat)width
{
    return self.frame.size.width;
}

- (void)setWidth:(CGFloat)width
{
    CGRect rect = self.frame;
    rect.size.width = width;
    self.frame = rect;
}

- (CGFloat)x
{
    return self.frame.origin.x;
}

- (void)setX:(CGFloat)x
{
    CGRect rect = self.frame;
    rect.origin.x = x;
    self.frame = rect;
}

- (CGFloat)y
{
    return self.frame.origin.y;
}

- (void)setY:(CGFloat)y
{
    CGRect rect = self.frame;
    rect.origin.y = y;
    self.frame = rect;
}

- (CGFloat)centerX
{
    return self.center.x;
}

- (CGFloat)centerY
{
    return self.center.y;
}

- (void)setCenterX:(CGFloat)centerX
{
    CGPoint point = self.center;
    point.x = centerX;
    self.center = point;
}

- (void)setCenterY:(CGFloat)centerY
{
    CGPoint point = self.center;
    point.y = centerY;
    self.center = point;
}

- (UIWindow *)findTheMainWindow
{
    NSEnumerator *frontToBackWindows = [UIApplication.sharedApplication.windows reverseObjectEnumerator];
    for (UIWindow *window in frontToBackWindows) {
        BOOL windowOnMainScreen = window.screen == UIScreen.mainScreen;
        BOOL windowIsVisible = !window.hidden && window.alpha > 0;
        BOOL windowLevelSupported = (window.windowLevel >= UIWindowLevelNormal);
        //BOOL windowSizeIsEqualToScreen = (window.xl_width == XLScreenW && window.xl_height == XLScreenH);
        if(windowOnMainScreen && windowIsVisible && windowLevelSupported) {
            return window;
        }
    }
    
    //XLPBLog(@"XLPhotoBrowser在当前工程未匹配到合适的window,请根据工程架构酌情调整此方法,匹配最优窗口");
    //if (XLPhotoBrowserDebug) {
    //    NSAssert(false, @"XLPhotoBrowser在当前工程未匹配到window,请根据工程架构酌情调整findTheMainWindow方法,匹配最优窗口");
    //}
    
    UIWindow * delegateWindow = [[[UIApplication sharedApplication] delegate] window];
    return delegateWindow;
}
/*! @brief 展示并绑定block回调 */
- (void)showWithSelectedCompletion:(FSActionSheetHandler)selectedHandler {
    self.selectedHandler = selectedHandler;
    
    _backView = [[UIView alloc] init];
    _backView.alpha = 0;
    _backView.backgroundColor = [UIColor blackColor];
    _backView.userInteractionEnabled = _hideOnTouchOutside;
    _backView.translatesAutoresizingMaskIntoConstraints = NO;
    [_backView addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(backViewGesture)]];
    [_controllerView addSubview:_backView];
    [_controllerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_backView]|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_backView)]];
    [_controllerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_backView]|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_backView)]];
    
    [self.tableView reloadData];
    // 内容高度
    CGFloat contentHeight = self.tableView.contentSize.height;
    // 适配屏幕高度
    CGFloat contentMaxHeight = CGRectGetHeight(self.window.frame)*FSActionSheetContentMaxScale;
    if (contentHeight > contentMaxHeight) {
        self.tableView.scrollEnabled = YES;
        contentHeight = contentMaxHeight;
    }
    [_controllerView addSubview:self];
    
    CGFloat selfW = CGRectGetWidth(_controllerView.frame);
    CGFloat selfH = contentHeight;
    CGFloat selfX = 0;
    CGFloat selfY = CGRectGetMaxY(_controllerView.frame);
    self.frame = CGRectMake(selfX, selfY, selfW, selfH);
    
    //[self.window makeKeyAndVisible];
    
     UIWindow *window = [self findTheMainWindow];
    [window addSubview:self];
    
    
    [UIView animateWithDuration:0.25 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
        _backView.alpha   = 0.38;
        CGRect newFrame   = self.frame;
        newFrame.origin.y = CGRectGetMaxY(_controllerView.frame)-selfH;
        self.frame        = newFrame;
    } completion:^(BOOL finished) {
        // constraint
        //[_controllerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[self]|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(self)]];
        //[_controllerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[self]|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(self)]];
        //self.heightConstraint = [NSLayoutConstraint constraintWithItem:self attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1 constant:contentHeight];
        //[_controllerView addConstraint:_heightConstraint];
    }];
}

#pragma mark - getter
- (UIWindow *)window {
    if (_window) return _window;
    
    _window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    _window.windowLevel = UIWindowLevelStatusBar+1;
    _window.rootViewController = [[UIViewController alloc] init];
    self.controllerView = _window.rootViewController.view;
    
    return _window;
}
- (UITableView *)tableView {
    if (_tableView) return _tableView;
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.scrollEnabled = NO;
    _tableView.separatorStyle  = UITableViewCellSeparatorStyleNone;
    _tableView.backgroundColor = self.backgroundColor;
    _tableView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    
    [_tableView registerClass:[FSActionSheetCell class] forCellReuseIdentifier:kFSActionSheetCellIdentifier];
    
    if (_title.length > 0) {
        _tableView.tableHeaderView = [self headerView];
    }
    
    return _tableView;
}

// tableHeaderView作为title部分
- (UIView *)headerView {
    UIView *headerView = [[UIView alloc] init];
    headerView.backgroundColor = FSColorWithString(FSActionSheetRowNormalColor);
    
    // 标题
    UILabel *titleLabel = [[UILabel alloc] init];
    titleLabel.numberOfLines = 0;
    titleLabel.backgroundColor = headerView.backgroundColor;
    titleLabel.translatesAutoresizingMaskIntoConstraints = NO;
    [headerView addSubview:titleLabel];
    self.titleLabel = titleLabel;
    // 设置富文本标题内容
    [self updateTitleAttributeText];
    
    // 标题内容边距 (ps: 要修改这个边距不要在这里修改这个labelMargin, 要到配置类中修改 FSActionSheetDefaultMargin, 不然可能出现界面适配错乱).
    CGFloat labelMargin = FSActionSheetDefaultMargin;
    // 计算内容高度
    CGFloat headerHeight = [self heightForHeaderView];
    headerView.frame = CGRectMake(0, 0, CGRectGetWidth(self.window.frame), headerHeight);
    
    // titleLabel constraint
    [headerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-labelMargin-[titleLabel]-labelMargin-|" options:0 metrics:@{@"labelMargin":@(labelMargin)} views:NSDictionaryOfVariableBindings(titleLabel)]];
    [headerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-labelMargin-[titleLabel]" options:0 metrics:@{@"labelMargin":@(labelMargin)} views:NSDictionaryOfVariableBindings(titleLabel)]];
    
    return headerView;
}

#pragma mark - setter
- (void)setContentAlignment:(FSContentAlignment)contentAlignment {
    if (_contentAlignment != contentAlignment) {
        _contentAlignment = contentAlignment;
        [self updateTitleAttributeText];
    }
}

#pragma mark - delegate
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 2;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return (section == 1)?1:_items.count;
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return FSActionSheetRowHeight;
}
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return (section == 1)?kFSActionSheetSectionHeight:CGFLOAT_MIN;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    FSActionSheetCell *cell = [tableView dequeueReusableCellWithIdentifier:kFSActionSheetCellIdentifier];
    return cell;
}
- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    FSActionSheetCell *sheetCell = (FSActionSheetCell *)cell;
    if (indexPath.section == 0) {
        sheetCell.item = _items[indexPath.row];
        sheetCell.hideTopLine = NO;
        // 当有标题时隐藏第一单元格的顶部线条
        if (indexPath.row == 0 && (!_title || _title.length == 0)) {
            sheetCell.hideTopLine = YES;
        }
    } else {
        // 默认取消的单元格没有附带icon
        FSActionSheetItem *cancelItem = FSActionSheetTitleItemMake(FSActionSheetTypeNormal, _cancelTitle);
        // 如果其它单元格中附带icon的话则添加上默认的取消icon.
        for (FSActionSheetItem *item in _items) {
            if (item.image) {
                cancelItem = FSActionSheetTitleWithImageItemMake(FSActionSheetTypeNormal, [UIImage imageNamed:@"FSActionSheet_cancel"], _cancelTitle);
                break;
            }
        }
        sheetCell.item = cancelItem;
        sheetCell.hideTopLine = YES;
    }
    sheetCell.contentAlignment = _contentAlignment;
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    // 延迟0.1秒隐藏让用户既看到点击效果又不影响体验
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1*NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        __weak __typeof(&*self)weakSelf = self;
        [self hideWithCompletion:^{
            if (indexPath.section == 0) {
                if (_selectedHandler) {
                    _selectedHandler(indexPath.row);
                }
                if (weakSelf.delegate && [weakSelf.delegate respondsToSelector:@selector(FSActionSheet:selectedIndex:)]) {
                    [weakSelf.delegate FSActionSheet:self selectedIndex:indexPath.row];
                }
            }
        }];
    });
}

@end

1:
创建工程
new —- project —- single view application
不要管Main.storyboard   ViewController.h  ViewController.m
2:
修改 “LaunchScreen.storyboard”，添加一个图像，作为启动界面 launch.png launch@2x.png launch@3x.png

3:
添加一个登陆viewcontroller，启动界面完成后跳到该页面
在AppDelegate.h
@property (retain,nonatomic) LoginViewController *loginView;
在AppDelegate.m 的didFinishLaunchingWithOptions

    self.loginView = [[LoginViewController alloc]initWithNibName:@"LoginViewController" bundle:nil];
    UINavigationController *navLoginView = [[UINavigationController alloc]initWithRootViewController:self.loginView];
    
    self.window.rootViewController = navLoginView;
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
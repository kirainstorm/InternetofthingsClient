package com.platform.util;

import android.content.Context;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

//文件操作工具类
public class FileOperateUtil {
	public final static String TAG="FileOperateUtil";
	 //获取目标文件夹内指定后缀名的文件数组,按照修改日期排序
	 //file 目标文件夹路径
	 //format 指定后缀名
	public static List<File> listFiles(String file,final String format){
		return listFiles(new File(file), format);
	}
	//获取目标文件夹内指定后缀名的文件数组,按照修改日期排序
	public static List<File> listFiles(File file,final String extension){
		File[] files=null;
		if(file==null||!file.exists()||!file.isDirectory())
			return null;
		files=file.listFiles(new FilenameFilter() {
			@Override
			public boolean accept(File arg0, String arg1) {
				return arg1.endsWith(extension);
			}
		});
		if(files!=null){
			List<File> list=new ArrayList<File>(Arrays.asList(files));
			sortList(list, false);
			return list;
		}
		return null;
	}

	//根据修改时间为文件列表排序
	//list 排序的文件列表
	//asc  是否升序排序 true为升序 false为降序
	public static void sortList(List<File> list,final boolean asc){
		Collections.sort(list, new Comparator<File>() {
			public int compare(File file, File newFile) {
				if (file.lastModified() > newFile.lastModified()) {
					if(asc){
						return 1;
					}else {
						return -1;
					}
				} else if (file.lastModified() == newFile.lastModified()) {
					return 0;
				} else {
					if(asc){
						return -1;
					}else {
						return 1;
					}
				}
			}
		});
	}
	//删除文件
	public static boolean deleteFile(String thumbPath,Context context) {
		boolean flag = false;
		File file = new File(thumbPath);
		if (!file.exists()) {
            //文件不存在直接返回
			return flag;
		}
		flag = file.delete();
		return flag;
	}
}
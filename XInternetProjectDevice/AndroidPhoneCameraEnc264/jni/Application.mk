#APP_STL := stlport_static

#ʹ��stlport_staticѡ��ʱ����boostһ��ʹ�ã���������ʱ�������´���
# undefined reference to 'std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string()'  
APP_STL := gnustl_static #stlport_static
APP_ABI := armeabi-v7a

常到vckbase 上找些小程序看看，获益不浅，近来找了找自动更新的程序，还没有写的太好的
索性用了两天时间，做了这个版本，有服务器配置及客户端更新工具，希望对有这方面需求的朋友有一定的帮助。

当然是初次写这玩儿，还有更多功能没有实现，更多BUG没有发现，请大家测试时多提建议，谢谢！

下面是配置文件说明：

作者：Kevin
MSN:fuwenke@gmail.com
网站:http://www.ourcode.net

AutoUpdateClient.exe
运行参数 服务器列表文件URL [auto]
auto 为可选项，如果为 auto, 则自动选择服务器更新,否则手工选择
例子：
	AutoUpdateClient.exe http://soft.ourcode.net/autoupdate/serverlist.txt auto
	
	服务器列表文件 存储格式：
	服务器名 + "|" + 文件列表URL
	
	例子：
	上海电信 videobbs.com [中国视频论坛提供下载]|http://www.videobbs.com/autoupdate/leapftp/leapftpupdate.xml
	重庆电信 sohu.com [中国SOHU公司提供下载]|http://www.videobbs.com/autoupdate/leapftp/leapftpupdate.xml
	
		文件列表 存储格式：
		用AutoUpdateConfig 生成，为标准 XML 文件
		
		例子(用CONFIG文件生成后，不建议手工进行修改)：
<?xml version="1.0" encoding="GB2312" ?> 
<xmlRoot keyname="KeyValue">
	<Product productname="Product-Name" createdate="2006-03-05 23:05:14" filenum="6">
		<File>
			<FileName>LeapFTP.exe</FileName>
			<FileVersion>2.7.5.610</FileVersion>
			<ProductVersion>2.7.5.610</ProductVersion>
			<Length>936448</Length>
			<FileDateLow>2320854784</FileDateLow>
			<FileDateHigh>29660771</FileDateHigh>
			<InstallDir>CURRENTDIR</InstallDir>
			<Register>NO</Register>
			<ZipFile>NO</ZipFile>
			<ForceOverwrite>NO</ForceOverwrite>
			<Execute>NO</Execute>
			<ExecutePar></ExecutePar>
			<LocalPath></LocalPath>
			<URL>http://videobbs.com/autoupdate/leapftp//LeapFTP.exe</URL>
			<UserName></UserName>
			<Password></Password>
		</File>
	</Product>
</xmlRoot>



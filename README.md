# http服务器搭建
>搭建这个服务器的原因是兴趣所在；服务器很简陋，以后将一步步优化，加油！！<br>
>每日更新优化的内容位于该文件的最下方<br>
### 下载仓储
<pre>
    使用Ubuntu git工具，克隆我的工程代码：
    git clone https://github.com/S-xiejiahui/http_v2.1.git 
    ps： 1、Ubuntu要能联网，2、如果不能联网，点击上方的Code下载zip压缩包
</pre>
### 服务器怎么运行：
<pre>
    1、make(make all)编译工程，将会生成一个 http 的可执行文件
    2、运行服务器：./bin/http（默认端口号8080）
      或者 ./bin/http 8080（8080指服务器的端口号，可自行指定）
    3、访问服务器：浏览器输入 http://Ip地址:8080/app.html (ex：http://192.168.1.1:8080/app.html) 
      或者 http://localhost:8080/app.html (注意：ubuntu上可以，但是用Windows浏览器访问不了)
    4、服务器启动成功
</pre>
### 配置git-ssh，让Ubuntu能上传文件到我的GitHub
<pre>
    1：记得将下面的e-mail修改为你的邮箱，然后一直点回车，默认设置即可
    2：本地配置已设置完毕，等待我将你的ssh密钥加入GitHub即可
</pre>
<pre>
    ssh-keygen -t rsa -C "e-mail@qq.com"                //获取ssh-key密钥
    git config --global user.name "username"            //设置你的ssh名
    git config --global user.email e-mail@qq.com        //设置你的ssh邮箱
    cat .git/config                                     //查看你的设置是否生效
    cat /home/xjh/.ssh/id_rsa.pub                       //显示ssh密钥
</pre>
### 怎么提交代码:
<pre>
    git fetch                                           //同步远端服务器内容到本地分支
    git rebase origin master                            //如果有打印信息，说明你本地代码落后，GitHub上的代码
                                                        //则使用这条命令，同步
    git add ...                                         //添加修改的文件
    git commit -m "..."                                 //添加修改此次文件的备注
    git log --graph                                     //查看修改历史
    git push origin master                              //将修改的文件，推送到GitHub
</pre>
### 目录结构:
<pre>
    ├── Readme.md       // help
    ├── bin             // 可执行文件目录
    │ ├── http          // web服务器
    │ ├── video         // 视频监控服务器
    │ └── ...
    ├── html            // html页面代码
    ├── Makefile        // 编译工具链
    ├── c-web-server    // c语言web服务器文件
    │ ├── inc
    │ ├── src
    │ ├── lib
    │ ├── lib-h
    │ └── ...
    │ ├── ...
    │ └── ...
    └── ...
</pre>
### 每日更新内容（记录每一点进步）：
>[2021/10/13]优化：解决访问服务器，无法加载css和svg图片问题（终于找到错误了：用时两天）<br>
>[2021/10/14]优化：新增make update方法，解决下载代码后，js文件的ip地址不是本地地址，造成无法正确访问服务器的问题<br>
>[2021/10/24]升级：代码融合视频监控项目，迭代升级V2.0<br>

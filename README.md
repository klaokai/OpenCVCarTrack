# OpenCVCarTrack

----------

**`2016/4/1 14:36:40`**

1. 发现BUG，QT无法显示单通道图像：
经调查发现，是Mat转换QImage格式时，转换方式不正确。更改代码如下：
```C++
QImage VideoInfo::Mat2QImage(Mat &image)
{
    QImage img;

    if (image.channels()==3) {
        //3通道
        cvtColor(image, image, CV_BGR2RGB);
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    } else if (image.channels()==1) {
        //单通道
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_Indexed8);
    } else {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    }

    return img;
}
```

----------


**`2016/3/28 16:54:26`**

1. 发现BUG，错误信息如下：
	
	OpenCV Error: Bad flag (parameter or structure field) (Unrecognized or unsupported array type) in cvGetMat, file D:\opencv\sources\modules\core\src\array.cpp, line 2482
	
	Invalid parameter passed to C runtime function.
	
	Invalid parameter passed to C runtime function.
	
	Error - RtlWerpReportException failed with status code :-1073741823. Will try to launch the process directly
	
	terminate called after throwing an instance of 'cv::Exception'
	
	what(): D:\opencv\sources\modules\core\src\array.cpp: 2482: error: (-206) Unrecognized or unsupported array type in function cvGetMat

	>解决方案：
	>一开始以为是跟踪识别模块出了问题，但是后来发现其实是视频读取模块也有问题。
	>主要是Mat结构处理的时候为空，导致后续检测的时候程序对NULL的Mat结构无法处理，进而产生错误。
	>故而加入了数据检查来规避错误的发生
2. 发现视频总帧数和实际能处理的帧数不一致

>示例1
>
>![1446帧结果图1][1]
>![1446帧结果图2][2]

>示例2
>
>![13731帧结果图1][3]
>![13731帧结果图2][4]

这里我感到有些奇怪，可以发现第一个示例中1446帧只能读取出905帧，第二个示例中13731帧只能读取8592帧。其实，经计算发现能读取的约占总帧数的62.5%。

```mathjax!
\begin{equation}
\frac{905}{1446} \approx 0.6258644536652835\label{ref1}
\end{equation}
```

```mathjax!
\begin{equation}
\frac{8592}{13731} \approx 0.6257373825649989\label{ref2}
\end{equation}
```

于是下载了视频播放器[Potplayer](http://potplayer.daum.net/?lang=zh_CN "64位 版本1.6.59347")来逐帧播放，发现即使是播放器也不是一帧一帧播放的，它有时候会跳过某一帧直接播放下一帧。而且有一定规律。
规律是12122，即首先会播放下一帧，其次跳过一帧播放，再次播放下一帧、从次跳过一帧播放，最后跳过一帧播放，如此循环将整个视频播放完。
故而这里不算是BUG，只是该格式是如此解码。
但是令我不安的是帧的不连续会导致图像抖动增大，减少我检测的精确度。这里我还需要很多改进。



  [1]: http://imgsrc.baidu.com/forum/w=580/sign=134ff677d02a60595210e1121835342d/958292eef01f3a296fb5cf709e25bc315d607c60.jpg
  [2]: http://imgsrc.baidu.com/forum/w=580/sign=1b4f804d7e310a55c424defc87444387/89849e22720e0cf33bb354450d46f21fbf09aa1a.jpg
  [3]: http://imgsrc.baidu.com/forum/w=580/sign=a20ab1df08b30f24359aec0bf894d192/93c5652762d0f703480c8f620ffa513d2797c52c.jpg
  [4]: http://imgsrc.baidu.com/forum/w=580/sign=d43020aa45a7d933bfa8e47b9d4ad194/7b06f103918fa0ecd3797eb3219759ee3c6ddb2c.jpg

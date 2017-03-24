nNovel-FT
=========

#### Project status
弃坑，怎么讲呢，nspire 的机能实在是捉急：

1. 加载一个文泉驿微米黑到内存，这机器需要接近 15 秒
2. 将文本从 UTF-8 转到 Unicode 是一个 O(n) 的过程，用过之前的版本的人都知道对 nspire 来说扫一边文件是有多慢。
3. 用刚刚加载好的字体，渲染一屏文本（单个中文字符16x16），需要接近 1.5 秒，同时字体越大速度越慢（可以理解但是不能接受）
4. 什么都不做，只是给屏幕填充色彩，也只能达到 20fps
综上，个人觉得，这个平台还是继续使用 bitmap 字库比较合理，没有折腾 Freetype 的意义（如果是英文阅读器倒是可以考虑）；另外，没有使用者的情况下要继续开发是完全没动力的。

~~这大概是第一个弃掉的坑吧~~

#### Overview
A simple text reader runs on TI-nspire CX/CX-CAS with freetype and UTF-8 support.

#### Licenses
MIT

# UE4_LearnUClass
这个工程是UE4的UClass学习工程。有两个场景LearnUClass和LearnBPClass。


### LearnUClass场景
在这个场景上摆放了两个由同一个**C++类MyActor**生成的蓝图所生成的对象。
![在这里插入图片描述](https://img-blog.csdnimg.cn/7315fd16d3e54c3190235fd0a7de1bc4.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA5rC05puc5pel6bih,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)  
测试了这两个对象的**ClassDefaultObject、UClass、StaticClass**的情况。详见代码MyActor.cpp。  
对应的讲解文章为：  
[【UE·底层篇】一文搞懂StaticClass、GetClass和ClassDefaultObject](https://zhuanlan.zhihu.com/p/380809095)

### LearnBPClass场景
自定义了一个**UserWidget类（MyUserWidget）**，然后建两个UMG蓝图，第一个的ParentClass选为**MyUserWidget**，名称叫UMG_Parent。第二个蓝图名称为UMG_Child，ParentClass选为**UMG_Parent**。然后在**LearnBPClass关卡蓝图**里生成这个UMG。![在这里插入图片描述](https://img-blog.csdnimg.cn/71a024d6e40249938508a6f2a8fd008e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA5rC05puc5pel6bih,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)
在**MyUserWidget**的代码里对下面类进行了测试：
- **运行时对象的GetClass和GetClass的父类以及父类的父类**
- 加载蓝图的GeneratedClass，**对不同蓝图的GeneratedClass调用GetClass进行比较**
- 加载蓝图
	- 蓝图的**GenerateClass、ParentClass、ParentNativeClass**
	- 蓝图的**GenerateClass的父类、GenerateClass的ParentNativeClass**

![在这里插入图片描述](https://img-blog.csdnimg.cn/95b2b3315ed64a238bd3b7aaf3e28895.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBA5rC05puc5pel6bih,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)  
对应的讲解文章为：  
[【UE·蓝图底层篇】一文搞懂NativeClass、GeneratedClass、BlueprintClass、ParentClass](https://zhuanlan.zhihu.com/p/438501014)

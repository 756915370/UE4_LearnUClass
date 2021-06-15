# 【UE·底层篇】一文搞懂StaticClass、GetClass和ClassDefaultObject
 学UE也有一段时间了，关于**StaticClass**和**GetClass**一直搞得不太懂。看别人代码时也莫名其妙，为什么这里用StaticClass？为什么那里用GetClass？  这都是啥玩意？  
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614213815298.jpg#pic_center)


正好最近看 **《大象无形 虚幻引擎程序设计浅析》** 还看到了**ClassDefaultObject**。正好把这三个东西彻底研究了一番。这篇文章我会尽力讲的连新手也能看懂，中间会少量掺杂ue4底层的一些源码。（顺带写了一些测试代码，[github工程。](https://github.com/756915370/UE4_LearnUClass)）

***

#### UClass和反射

什么是反射？不是光学那个反射，是编程语言的那种反射。有c#或者java使用经验的肯定知道**反射**这个东西。简单来说，**反射的作用就是在不知道这个类是什么类的情况下获取到它的一些信息。** C++是没有反射机制的所以UE底层实现了一套反射机制。C#怎么用反射：  

```
string s = "鸡桑大帅逼”；
Type t = s.GetType()；
```

Type里面就包含了string这个类的各种信息。接下来请我和大声念三遍：

- **UClass就是C#里的Type!**
- **UClass就是C#里的Type!**
- **UClass就是C#里的Type!**  

虽然UClass和C#里的Type的作用并不完全相同，但是这么理解会帮我们弄明白UE底层的很多事情。

***

#### GetClass()

现在我们对UClass是干嘛用的已经有了一个初步的了解，来看GetClass。**GetClass()** 的作用就是我们生成一个UObject实例后，去拿这个实例的UClass，类似C#的**GetType**。接下来我们做个小实验。新建一个自己的Actor就叫MyActor好了，写一个方法并且让它蓝图可调用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614222623757.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)
建两个蓝图，并拖到场景中，在关卡蓝图里调用**IsSameUClass**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614223124274.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614223133922.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)
显示的结果是0，也就是两个**MyActor的GetClass()结果不一样**。但是如果场景上的两个Actor**都是BP_MyActor同一个蓝图，它们的GetClass()结果是一样的。**  
先解释第一个结果：**为什么同样C++类型不同蓝图返回的GetClass()结果不一样？** 那是因为UClass不仅需要记录类型信息，还需要承担**序列化**的工作。同样C++类型，但是蓝图不一样，需要序列化数据是不一样的，所以UClass不一样。同样类型，同样蓝图，但是场景中实例不一样，但是这时需要序列化的蓝图数据是一样的（像场景中的位置信息不是记录在蓝图里的），所以它们的UClass是一样的，这是第二个结果的原因。

***

#### GetStaticClass()

GetClass很好很强大。但是有个问题，**它是UObject的成员函数**。我现在没有UObject实例，但是我想拿到某个类的UClass怎么办？锵锵，**StaticClass**来了。我可以这么调用：

```
UClass* myUClass = AMyActor::StaticClass();
```

这样子我就**不需要有UObject实例也能拿到某个类的UClass了**。同时因为它是Static的所以每次调用T::StaticClass返回的都是同一个结果。

```
auto static1Class = AMyActor::StaticClass();
auto static2Class = AMyActor::StaticClass();
auto result = static1Class == static2Class;
//结果是1
UE_LOG(LogTemp, Warning, TEXT("Is StaticClass Same: %d"), result);
```

再讲StaticClass的另一个功能，帮助大家更加理解StaticClass。先来看UObject、UClass、Actor类图（虽然中间有一些类名大家可能没看过但是没有关西）：  
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615145913677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)

现在又有个问题来了，我现在有一个**UClass，如何知道这个UClass和另一个UClass是不是继承关系？** 直接转类型判断吗？不对，看上面那张图，UClass是没有子类的。那怎么办？答案是**UClass存储了它要描述的类的父类的StaticClass。** 有点绕，我举例说明一下。我现在有个MyActor类的UClass，想知道MyActor是不是UObject的子类，我就这么做：

- 调用UObject::StaticClass()
- 拿到MyActor的UClass存储的MyActor的父类的StaticClass也就是Actor的StaticClass
- 比较Actor的StaticClass和UObject的StaticClass是不是相等
- 发现不相等，这次我拿Actor的StaticClass（因为它也是一个UClass）里存的Actor父类的StaticClass也就是UObject的StaticClass，然后比较，发现相等，返回true
- 如果我发现不相等，我就一直比，比到最后没有父类，返回false

上面这段逻辑就是**UClass的IsChildOf的实现原理**。调用IsChildOf的时候传入泛型：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614232758839.png#pic_center)
这是具体实现。**UStruct**是UClass父类。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614232852774.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)
GetSuperStruct()就是返回它存储的父类的StaticClass。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210614233221277.png#pic_center)

顺带再讲一下**IsA**这个函数，功能和IsChildOf一样，不同的是IsChildOf给UClass用的，IsA是给UObject对象实例用的，比如想知道一个MyActor实例是不是UObject的子类：

```
//myActor是MyAcor*类型
bool result = myActor->IsA<UObject>();
```

***

#### ClassDefaultObject

**GetClass**和**GetStaticClass**都明白了之后，理解**ClassDefaultObject** 就简单多了。ClassDefaultObject（简称CDO），类默认对象。通过CDO我可以拿到**一个UObject初始化时的值**。虽然CDO有个Default默认，但是用默认去描述它的功能不是很准确。回到我们创建的类MyActor，给它添加一个外部可以修改变量**testIntValue**，默认值1:  
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615135825711.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)  
对于两个蓝图分别修改，一个20，另一个10：  
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615140019393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615135947211.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)  
然后在BeginPlay写下测试代码：  

```
//先修改值再获取ClassDefaultObject
testIntValue = -1;
auto defaultInt = GetClass()->GetDefaultObject<AMyActor>()->testIntValue;
UE_LOG(LogTemp, Warning, TEXT("DefaultObject int: %d"), defaultInt);
```

最后打印的结果是一个20，一个10。  
**如果想拿到代码里给它设的默认值1要用StaticClass**：

```
auto staticDefaultInt = AMyActor::StaticClass()->GetDefaultObject<AMyActor>()->testIntValue;
//打印1
UE_LOG(LogTemp, Warning, TEXT("StaticDefaultObject int: %d"), staticDefaultInt);
```

***

#### GetClass()->GetClass()->GetClass()

最后来点丧心病狂的东西。上面我画了一张类图，UClass也是UObject的子类。  
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615145913677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)  
一个UObject实例可以调用GetClass()来获得它的UClass，那么对UClass调用GetClass()会出现什么结果呢？在MyActor里我这么写：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615141626426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)  
结果如下：  
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210615141730515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2o3NTY5MTUzNzA=,size_16,color_FFFFFF,t_70#pic_center)  
**调用第一次GetClass和第二次产生的结果不一样，第三次以后结果是一样的了**。至于为什么会这样我不知道，网上也没查出个所以然来，如果有谁弄懂请告诉我，我这里就先记录下来。

***

#### 总结

- **UClass。存储类信息，用于反射。把它当成C#的Type来理解。**
- **GetClass()。获得一个UObject实例的UClass，是UObject成员函数。**
- **GetStaticClass()。不需要有实例就能获得UClass。是静态的，每次调用返回相同结果。**
- **ClassDefaultObject。类默认对象，可以获得UObject初始化时的值。注意GetClass()->GetDefaultObject()和T::StaticClass()->GetDefaultObject()不一样。**

***

关于作者

- **水曜日鸡**，喜欢ACG的游戏程序员。曾参与索尼中国之星项目《硬核机甲》的开发。 目前在某大厂做UE4项目。

CSDN博客：[https://blog.csdn.net/j756915370](https://blog.csdn.net/j756915370)  
知乎专栏：[https://zhuanlan.zhihu.com/c_1241442143220363264](https://zhuanlan.zhihu.com/c_1241442143220363264)  
游戏同行聊天群：891809847

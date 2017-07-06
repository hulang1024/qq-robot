# qq-robot
基于CQ的多功能QQ聊天机器人。


### 目前支持:
* 执行JS程序，语法：`eval: <code>`，例如发送以下消息:
    ```js
    eval: "xyz".length
    ``` 
* 黑名单操作  
    ```scheme
    !black add 1013644379
    !black del 1013644379
    !black clear
    !black list
    ```
* 互@对方
* 机器人接收到的私聊消息自动转发给管理者
* 管理者可以让机器人代发消息
    ```scheme
    !sendtogroup 群号 [可选的QQ号] 消息
    !send QQ号 消息
    ```
* osu!查询
    ```scheme
    !stat <username>
    ```
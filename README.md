# qq-robot
基于CQ的多功能QQ聊天机器人。


### 目前支持:
* 执行JS程序，语法：`eval: <code>`，例如发送以下消息:
    ```js
    eval:
    function add(x, y) { return y == 0 ? x : add(x + 1, y - 1); };
    add(1, 10)
    ``` 

* 黑名单操作  
    ```scheme
    !black add 1013644379
    !black del 1013644379
    !black clear
    !black list
    ```

* 互@对方
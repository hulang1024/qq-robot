# qq-robot
基于酷Q Air，QQ多功能机器人。


### 目前支持:
* 作为JS群内交互式环境，语法：`eval: <js-code>`，例如发送以下消息:
    ```js
    eval:
    function add(x, y) { return y == 0 ? x : add(x + 1, y - 1); };
    add(1, 10)
    ``` 
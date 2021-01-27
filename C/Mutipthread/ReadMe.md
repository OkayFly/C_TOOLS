sleep是Thread类中的静态方法。无论是在a线程中调用b的sleep方法，还是b线程中调用a的sleep方法，谁调用，谁睡觉。最主要的是sleep方法调用之后，并没有释放锁，sleep是帮助其他线程获得运行机会的最好方法，但是如果当前线程获取到的有锁，sleep不会让出锁。使得线程仍然可以同步控制。sleep不会让出系统资源；而wait是进入线程等待池中等待，让出系统资源，释放对象锁。

一个sleep 一个没有（每释放资源sleep） 会造成一直执行一个

    while(1)
    {
       
        pthread_mutex_lock(&mut);
        printf("thead2\n");
         
        // x++;
        if(x%y == 0)
        {
            // pthread_cond_broadcast(&cond);
            printf("pthread_cond_signal\n");
            pthread_cond_signal(&cond);
            //pthread_cond_signal函数的作用是发送一个信号给另外一个正在处于阻塞等待状态的线程,使其脱离阻塞状态,继续执行.如果没有线程处在阻塞等待状态,pthread_cond_signal也会成功返回。
        }
      x++; //TODO(Zhangx) xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

       printf("!!!!!!!!!!!x:%d\n",x);
        pthread_mutex_unlock(&mut);
         sleep(1);
    }
    https://www.cnblogs.com/x_wukong/p/7909895.htm
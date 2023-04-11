深度休眠时，power key是否可以唤醒？
深度休眠时，点tp是否有 中断？

深度休眠唤醒需要设唤醒 io

suspend
        irq = gpio_to_irq(pdata->hall_switch_gpio.io);
        enable_irq_wake(irq);

resume
        irq = gpio_to_irq(pdata->hall_switch_gpio.io);
        disable_irq_wake(irq);

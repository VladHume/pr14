# pr14

## Мета роботи

Реалізувати програму мовою C, яка використовує POSIX-таймери (на базі `timer_create`), і дозволяє **динамічно змінювати інтервал таймера під час виконання програми**, без її перезапуску.

## Теоретичні відомості

### POSIX-таймери

Сучасний механізм, який дозволяє створювати кілька високоточних таймерів, з можливістю:

- Виклику обробника сигналу (`SIGEV_SIGNAL`) або потоку (`SIGEV_THREAD`)
- Отримання точного контролю над періодом і залишковим часом
- Підтримки інтервальних і одноразових таймерів

### Основні функції:

- `timer_create()` — створює таймер
- `timer_settime()` — запускає або змінює таймер
- `timer_gettime()` — отримує статус таймера
- `timer_delete()` — видаляє таймер

## Реалізація

### Зміна інтервалу під час виконання

Механізм реалізовано за допомогою окремого потоку (`pthread`), який зчитує нове значення інтервалу з `stdin` і оновлює існуючий таймер за допомогою `timer_settime()`.

Використовується сигнал `SIGRTMIN` для уникнення конфліктів з іншими сигналами.

### [Код](https://github.com/VladHume/pr14/blob/main/dynamic_timer.c) програми

## 💻 Компіляція

```sh
gcc -Wall -o dynamic_timer dynamic_timer.c -lrt -pthread
```

---

## Приклад використання

- Запуск програми: кожну секунду виводиться `Tick`
- Введення в stdin нового числа (наприклад, `3`) — інтервал оновлюється на 3 секунди
- Всі зміни виконуються без завершення або перезапуску програми
  
  ![image](https://github.com/user-attachments/assets/80a44b40-fbc9-4c55-8feb-58b7d29e93da)


---

## Висновки

- Програма демонструє ефективне використання POSIX-таймерів з динамічним керуванням інтервалом.
- Забезпечено розділення таймерного обробника і потоку вводу.

# RoomSched
Инициализация базы данных (первый вариант - тестовый):

1. Запустить PostgreSQL:
   sudo service postgresql start
   sudo -u postgres psql < backend/postgresql/database_main.sql

2. Простой тест:
   psql -d roomsched -c "SELECT * FROM test;

   Так компилировать С++ файл:
   sudo -u postgres ./a.out
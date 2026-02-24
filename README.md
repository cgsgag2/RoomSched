# RoomSched
Introduction for using postgresql in 'roomsched' project
1. sudo -u postgres psql -f create_database.sql
2. psql -U rsched_user -d roomsched -h localhost // Check connection
3. PGPASSWORD='...' psql -h localhost -U rsched_user -d roomsched -f create_tables.sql
4. Compile (temporary):  
g++-12 -std=c++20 -pedantic-errors -Wall -Wextra -Werror -fsanitize=undefined src/db_manager.cpp src/db_users.cpp db_main.cpp -o roomsched -lpqxx -lpq

5. Run programm: ./roomsched
---
Introduction for using client-part in 'roomsched' project
1. Download Qt 6.x (MSVC 64-bit)
2. cd /d D:\project\RoomSched\frontend\client    // enter your path to files
3. rmdir /s /q build
   mkdir build   //optional comands for clear build
4. cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
5. cmake --build .
6. "C:\Qt\6.10.2\msvc2022_64\bin\windeployqt.exe" RoomSchedClient.exe  // your path to Qt in the beginning
7. RoomSchedClient.exe  // just type this in terminal after all and enjoy
---
How to use together DB and server:
1. sudo -u postgres psql -f create_database.sql
2. psql -U rsched_user -d roomsched -h localhost // Check connection
3. PGPASSWORD='...' psql -h localhost -U rsched_user -d roomsched -f create_tables.sql
4. cmake .. // From /build
5. cmake --build .
6. ./backend/server/server

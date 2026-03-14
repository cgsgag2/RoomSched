# RoomSched
---
How to use together DB and server:   
1. sudo -u postgres psql -f create_database.sql
2. psql -U rsched_user -d roomsched -h localhost // Check connection
3. PGPASSWORD='...' psql -h localhost -U rsched_user -d roomsched -f create_tables.sql
4. cd backend
5. cmake -S . -B build 
6. cmake --build build
7. ./build/server/server

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

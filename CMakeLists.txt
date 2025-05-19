cmake_minimum_required(VERSION 3.28)
project(TicketSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        database.h
        main.cpp
        user.h
        train.h
        my_time.h
        ticket.h)
all:
	g++ main.cpp CreateDisk.cpp Tables.cpp Utils.cpp Select.cpp BasicFileSystem.cpp Insert.cpp -o main.o
	./main.o

copypasta:
	create database kamil.dbo 200kb
	create table Empleado -columns=codigo,nombre,edad -types=int,char(100),double -key=codigo
	insert Empleado -columns=codigo,edad -values=500,198.89
	select Empleado -columns=codigo,edad -where=codigo<>1
	select Cliente -columns=* -where=codigo<>1

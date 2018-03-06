all:
	g++ main.cpp CreateDisk.cpp Tables.cpp Utils.cpp BasicFileSystem.cpp -o main.o
	./main.o

copypasta:
	create database edgar2.dbo 200kb
	create table Client -columns=codigo,nombre,direccion,edad -types=int,char(100),char(1000),double -key=codigo
	insert Final -columns=codigo,nombre,direccion -values=1,’valor 1’,’direccion1’

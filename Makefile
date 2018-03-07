all:
	g++ main.cpp CreateDisk.cpp Tables.cpp Utils.cpp Select.cpp BasicFileSystem.cpp -o main.o
	./main.o

copypasta:
	create database jona.dbo 200kb
	create table Chiqui -columns=codigo,nombre,edad -types=int,char(100),double -key=codigo
	insert Kamil -columns=codigo,nombre,edad -values=600,este no haha,88
	select Kamil -columns=codigo,nombre,direccion -where=codigo<>1

all:
	g++ main.cpp CreateDisk.cpp Tables.cpp Utils.cpp Select.cpp BasicFileSystem.cpp Insert.cpp Update.cpp Delete.cpp -o main.o
	./main.o

copypasta:
	create database kamil.dbo 200kb
	create table Cliente -columns=codigo,nombre,edad -types=int,char(100),double -key=codigo
	insert Cliente -columns=codigo,nombre,edad -values=25, que pena , 88.88
	insert Cliente -columns=nombre,edad -values=hola, 12
	insert Cliente -columns=codigo,nombre,edad -values=13, bien y vos , 90.90
	insert Cliente -columns=codigo,nombre,edad -values=34, bieen aqui , 50.05
	insert Cliente -columns=codigo,nombre,edad -values=22, programando y vos? , 10.40
	insert Empleado -columns=nombre -values=ni te vas a graduar loco
	insert Empleado -columns=nombre -values=yose viejo pero ni modo
	insert Empleado -columns=nombre -values=cagada chele
	insert Empleado -columns=nombre -values=pije cagad


	select Cliente -columns=codigo,nombre,edad -where=codigo > 1
	select Cliente -columns=* -where=codigo > 20

	select Cliente -columns=* -where=codigo > 20
	select Empleado -columns=* -where=codigo > 1
	update Cliente -columns=nombre,edad -values=’Me equivoque’,111.12 -where=codigo > 21
	delete Cliente -where=codigo = 25
	drop table Cliente

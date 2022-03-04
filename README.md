# PROJET ROBOT

:warning: **ESEO School Project** *(not all resources are comitted)* :warning:

This project is designed for ESEO engineering students in the embedded systems option (SE).</br>
In this project, you will learn about the C programming language, how it works, the right way to develop it, and the correct naming conventions.</br>
**Projet Robot** works with **Intox simulator** and **Infox librairies**.


# TECH

**Projet Robot** is made to work with ESEO school ressources.

- **C Programming Language** - This **Projet Robot** is developed with the C programming Language.
- **Intox** - ESEO Java simulator, to have a graphic view of the project in operation.
- **Infox** - ESEO C Language libraries, these libraries are necessary for this project to run smoothly.


# INSTALLATION

Before cloning this repository, you need to download all of the necessary ESEO resources for this project :
- **Intox simulator**
- **Infox librairies**


Firstly, you need to clone this project :

```sh
git clone git@github.com:Tibibouu/projet_robot.git
```


Secondly, to generate the executable.</br>
Go in the *projet_robot* folder, run `make` command, and do the same for commando :

```sh
cd telco/
make
cd ../commando/
make
```

You can find the executables in the *bin* folders.


Then, you need to run the **ESEO Java simulator**.</br>
Go to your parent folder of this project, where the **ESEO Java simulator Intox** and the **ESEO librairies Infox** are located, and run it :

```sh
cd ..
java -jar intox-java_1.8-v0.3.6.jar
```


Finally, you need to run this project.</br>
Go to the *commando bin* folder *(executable folder)*, and run the executable</br>
Go to the *telco bin* folder *(executable folder)*, and run the executable :

```sh
cd commando/bin/
./commando
cd telco/bin/
./telco
```

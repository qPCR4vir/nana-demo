# nana-demo
[Linux (gcc 5.2.1 and 4.9.2)![TravisCI build status](https://travis-ci.org/qPCR4vir/nana-demo.svg)](https://travis-ci.org/qPCR4vir/nana-demo)  including [(nana)](https://github.com/qPCR4vir/nana)

This is a collection of small examples borrowed (with modifications) from any possible source, mostly from documents and blogs posted by [Jin Hao](https://github.com/cnjinhao).  

The simple applications in the root directory show how to combine a few features of [Nana C++ GUI programing](https://github.com/cnjinhao/nana).  

The aim of each .cpp files in the "Examples" directory is to show how to use one feature.  

The __master__ branch allow you to quickly browse, modify and compile any of the demos. You can use it for a quick setup of your own project.  
The branch __dev_nana_in examples__ introduce a dependency of each demo project on the source code of nana and add the project for the generation of the nana static library to the solutions for Visual Studio 2013 and 2015. This allow you to quickly select (checkout) the exact version of nana (in the nana repository!!) - for example, the latest hotfix - you want to use and recompile it together with the demo of your choice. It also allow you to quickly browse, modify, test and contribute! to nana.

All demos (FreeMe- not) are updated to Nana v1.4.  

Need periodical revision and adaptation.  

This is also a test for each change in nana. I compile regularly all of them after each push to the master or hotfixes branches of [Nana](https://github.com/cnjinhao/nana). Travis will automatically compile all (see the nana CMakeFile.txt) in a CI test in Linux with gcc:

 + nana/master will compile nana-demo/master 
 + nana/hotfix will compile nana-demo/dev_nana_in_examples  
 + nana/develope  - nana/stable.

All are automatically added to the Doxygen-generated [Nana Documentation](http://qpcr4vir.github.io/nana-doxy/html/index.html) from the project [Nana-docs](https://github.com/qPCR4vir/nana-docs)

If you have another build system tested for this collection of demos, please take a time to contribute it back here. Thank!

## Travis usage of branchs in *nana* and *nana-demo*:

### When compiling *cnjinhao/nana* (or *qPCR4vir/nana*) 
It will also compile some test from *qPCR4vir/nana-demo*.

nana branch                    |               call in .travis.yml
-------------------------------|----------------------------------------
cnjinhao/nana/**master**/      |  git clone --depth=1 --branch=**develop** https://github.com/qPCR4vir/nana-demo.git ../nana-demo
qPCR4vir/nana/**master**	      |  git clone --depth=1 --branch=**develop** https://github.com/qPCR4vir/nana-demo.git ../nana-demo
cnjinhao/nana/**hotfix-1.4.1**	|  git clone --depth=1 --branch=**develop** https://github.com/qPCR4vir/nana-demo.git ../nana-demo
qPCR4vir/nana/**hotfix-1.4.1** |  git clone --depth=1 --branch=**develop** https://github.com/qPCR4vir/nana-demo.git ../nana-demo
cnjinhao/nana/**develop**		    |  git clone --depth=1 --branch=**develop** https://github.com/qPCR4vir/nana-demo.git ../nana-demo			 
qPCR4vir/nana/**develop**		    |  git clone --depth=1 --branch=**develop** https://github.com/qPCR4vir/nana-demo.git ../nana-demo
						
						 
That is: all *nana* branchs use *nana-demo/develop*. 
This need to be fixed. Each *nana* branch have to use the corresponding branch of the demos.
	
	
	
### Travis is also used to test *nana-demo*. 
It need to compile first some *nana*. 

nana branch                 	|               call in .travis.yml
--------------------------------|----------------------------------------
nana-demo/**master**   				         |  git clone --depth=1 --branch=**master** https://github.com/cnjinhao/nana.git ../nana
nana-demo/**hotfix-1.4_with_nana**  |  git clone --depth=1 --branch=**hotfix-1.4** https://github.com/cnjinhao/nana.git ../nana
nana-demo/**hotfix-1.4**			         |  git clone --depth=1 --branch=**hotfix-1.4** https://github.com/cnjinhao/nana.git ../nana
nana-demo/**develop_with_nana**  	  |  git clone --depth=1 --branch=**tests** https://github.com/qPCR4vir/nana.git ../nana 
nana-demo/**develop**  			         	|  git clone --depth=1 --branch=**tests** https://github.com/qPCR4vir/nana.git ../nana 
						    
							
Upss... *nana-demo/develop* use some *nana/test* that don't exists any more. It need to be *nana/develop*.

# nana-demo
Continuous Integration Test - Travis: [Linux (gcc 8)![TravisCI build status](https://travis-ci.org/cnjinhao/nana.svg)](https://travis-ci.org/cnjinhao/nana) which tests both nana-demo and [(nana)](https://github.com/cnjinhao/nana)

This is a collection of small examples borrowed (with modifications) from any possible source, mostly from documents and blogs posted by [Jin Hao](https://github.com/cnjinhao).  

The simple applications (netepad, calculator, etc.) in the root directory show how to combine a few features of [Nana C++ GUI programing](https://github.com/cnjinhao/nana).  

The aim of each .cpp files in the "Examples" directory is to show how to use one Nana feature (and to test it).  

The *master*, *hotfix* and *develop* branches allow you to quickly browse, modify and compile any of the demos. 
You can use it for a quick setup of your own project. This allow you to quickly select (checkout) the exact version of nana (in the nana repository!!) - for example, 
the latest hotfix - you want to use and recompile it together with the demo of your choice. 
It also allow you to quickly browse, modify, test and contribute! to nana.

All demos (FreeMe- not) are updated to Nana v1.7.  But this need periodical revision and adaptation.  

This is also a test for each change in nana. I compile regularly all of them after each push to the *master* 
or *hotfixes* branches of [Nana](https://github.com/cnjinhao/nana). 
Travis will automatically compile all (see the nana `.travis.yml`) in a Continuous Integration test (CI) in Linux with gcc:

 + nana/master will compile nana-demo/master 
 + nana/hotfix will compile nana-demo/hotfix  
 + nana/develope  - nana/develope.

All demos are automatically added to the Doxygen-generated [Nana Documentation](http://qpcr4vir.github.io/nana-doxy/html/index.html) 
from the project [Nana-docs](https://github.com/qPCR4vir/nana-docs)

If you have another build system tested for this collection of demos, please take a time to contribute it back here. Thank!


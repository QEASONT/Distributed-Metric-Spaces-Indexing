# Distributed Metric Spaces Indexing For Similarity Search
-----------------------------------------------------------------------------------------------------------------
## Introduction
This is a source code for the algorithm described in the paper **Distributed Metric Spaces Indexing For Similarity Search**. 

## Compilation

This project is written by **C++** and is simple and easy to use. It can be complied by **g++** in **Linux** and **MSVC** in **Windows**.

### Dependency

In order to compile the program, you should have the dependencies of **Boost**, **Eigen**. For example, for Ubuntu users, you can install Boost with the following commands:
```bash
sudo apt-get install libboost-dev
sudo apt-get install libboost-program-options-dev
```
For Windows users, you can download Boost library and put it in the include path.


### Installation
#### Windows
We can use **Visual Studio 2021** (Other version of Visual Studio should also work but remains untested) to build the project with importing all the files in the directory 

#### Linux
```bash
cd ./xxx
make
```
## Dataset

In this project, the format of the input file (such as `d50s10000.bench, d50s10000.data_new`, which is in `double` data type) is the same as that in [LSHBOX](https://github.com/RSIA-LIESMARS-WHU/LSHBOX). It is a binary file but not a text file, because binary file has many advantages. The binary file is organized as the following format:

>{Bytes of the data type (int)} {The size of the vectors (int)} {The dimension of the vectors (int)} {All of the binary vector, arranged in turn (double)}

For other dataset we use, you can get the raw data from following links:

```bash
Link: https://pan.baidu.com/s/1ytQ_qLABqYv1oB3cM6oQUQ?pwd=iqv4 
Password: iqv4 
```

Next, you should transform your raw dataset into the mentioned binary format, then rename it is `[datasetName].data_new` and put it in the directory `./dataset`.

If you meet any issue on the code or take interest in this work, please feel free to contact Qian Tang (qiantang007@163.com). Thank you.

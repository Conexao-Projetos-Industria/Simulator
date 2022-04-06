# Simulator

## Commands to build on linux:

First install dependencies:
```
sudo apt install libglx-dev libgl1-mesa-dev libglfw3-dev libglew-dev cmake
```

Then build using cmake
```
mkdir build
cd build
cmake ..
cmake --build .
cd ..
```

## Commands to build on windows:
Dependencies are downloaded and built automatically.
Only Visual Studio generator is supported at the moment.

```
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build .
```

## Commands to run on linux:

./bin/sim_base model/pendulum/pendulum.xml

## Commands to run on windows:

```
bin\Debug\sim_base.exe model\pendulum\pendulum.xml
```

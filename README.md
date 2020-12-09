# Modern C++ features

Contains example of modern *C++* usage.

## How to run the code locally with *Bazel* already installed on host

*Basic examples:*

```bash
bazel test //modern_cpp_examples/basic_examples/...
bazel test //modern_cpp_examples/map/...
```

*Advanced examples:*

```bash
bazel test //modern_cpp_examples/advanced_examples/...
```

## Run the code inside a container

You can use the following Docker image to instantiate a container locally with Ubuntu and Bazel already installed:

```bash
docker run -it --rm framaxwlad/ubuntu_dev:latest
```

There you can simply clone the repository:

```bash
git clone https://github.com/FBorowiec/advanced_cpp.git
cd advanced_cpp/
```

And use the aforementioned commands to run the program:

```bash
bazel test //modern_cpp_examples/advanced_examples/...
```

# G-Shell

C Shell implementation for UCL 3rd year Operating Systems coursework.

## Features

* Load `$HOME` and `$PATH` from `profile` file
* `$HOME` and `$PATH` variable assignment
* Execute any commands located in `$PATH`

## Run

```
make

./shell
```

## Implementation

Read [docs.pdf](docs.pdf)

## Compiling docs

Use [pandoc](http://pandoc.org/).

```
pandoc docs/docs.md -o docs.pdf --toc -N
```

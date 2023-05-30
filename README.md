# CUDADFAGen
CUDADFAGen is a tool designed to automate the process of generating DFA transition tables for CUDA programs. It enables 
efficient parallel processing of lexical analysis on GPUs.

The objective of CUDADFAGen is to provide a convenient tool for the lexical analysis phase, similar to using Lex to create lexical analyzers but for CUDA.
Currently, CUDADFAGen supports keyword identification, where the <strong>syntax</strong> for keywords is defined as <strong>"keyword" with all lowercase letters.</strong> 

Feel free to explore and contribute to CUDADFAGen, and let us know your feedback and suggestions for further enhancements!


## How to Run
1. Execute ```make``` in the main directory: This command will generate a dfagenerator executable. The dfagenerator takes a list of keywords as input, which can be provided through a file.

2. Run ```./dfagen examples/test.dfa```: This command will generate a plex.cu file, which contains the DFA table ready to be used.

3.  Compile the generated plex.cu by running ```nvcc plex.cu -o plex```: This command compiles the plex.cu file and generates an executable named plex. This executable can be used to see it working. 

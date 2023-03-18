# Median filter and mean filter with input data buffer

HW1_Q2 : 
Median filter and mean filter fifo module with FIFO interfaces and extra input data buffer.

I choose "peppers_color.bmp", 
         "peppers_color_noise.bmp", 
         "woman_blonde.bmp" ,
         "woman_blonde_noise.bmp" four images to implement median and mean fifo.

In filter.cpp, data_buffer[3][6] is the input data buffer for storing pixels which sent from the Testbench.cpp .

Outputs are four images pass through median filter and mean filter, "peppers_color_medain_mean.bmp", 
                                                                    "peppers_color_noise_medain_mean.bmp", 
                                                                    "woman_blonde_medain_mean.bmp" ,
                                                                    "woman_blonde_noise_medain_mean.bmp" in "build" folder.

Algorithm of the code is demonstrated in the report.pdf in EEclass !

Remaining part is showing how to run my code:

1. cd build
2. cmake ..
3. make run


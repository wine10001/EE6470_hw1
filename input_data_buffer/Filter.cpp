#include <cmath>
#include <algorithm>
#include "Filter.h"

Filter::Filter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_median_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}


// sobel mask
const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, 
                                  {1, 2, 1}, 
                                  {1, 1, 1}};

void Filter::do_median_filter() {
  int data_buffer [3][6] = {0}; // Input data buffer

  while (true) {
    int box[3][9] = {0};
    int sorted_box[3][9] = {0};
    int result[3] = {0};

    // Check x cooridinate of the pixel is 0 or not
    if (i_flag.read()){                               // x cooridinate of the pixel is 0
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          unsigned char color[3] = {i_r.read(),  i_g.read(), i_b.read()};       
          for (unsigned int i = 0; i != 3; ++i) {
            box[i][v*3+u] = color[i];             
            sorted_box[i][v*3+u] = color[i]; 
          }
        }
      }
    } else{                                          // x cooridinate of the pixel is not 0
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        unsigned char color[3] = {i_r.read(),  i_g.read(), i_b.read()}; 
        for (unsigned int u = 0; u < MASK_X; ++u) {      
          for (unsigned int i = 0; i != 3; ++i) {
            if (u==2){
              box[i][v*3+u] = color[i];              
              sorted_box[i][v*3+u] = color[i]; 
            } else{
              box[i][v*3+u] = data_buffer[i][v*2+u];              
              sorted_box[i][v*3+u] = data_buffer[i][v*2+u]; 
            }
          }
        }
      }
    }

    // Store pixels in data_buffer
    for (unsigned int i = 0; i != 3; ++i) {
      data_buffer[i][0] = box[i][1];
      data_buffer[i][1] = box[i][2];
      data_buffer[i][2] = box[i][4];
      data_buffer[i][3] = box[i][5];
      data_buffer[i][4] = box[i][7];
      data_buffer[i][5] = box[i][8];
    }


    // Implement median filter
    for (unsigned int i = 0; i < 3; ++i) {
      std::sort(std::begin(sorted_box[i]), std::end(sorted_box[i]));
      box[i][4] = sorted_box[i][4];
    }

    // Implement mean filter
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        for (unsigned int i = 0; i != 3; ++i) {
          result[i] += box[i][v*3+u] * mask[u][v] / 10; 
        }
      }
    }

    // Write result
    o_result_r.write(result[0]);
    o_result_g.write(result[1]);
    o_result_b.write(result[2]);
    wait(1); //emulate module delay
  }
}

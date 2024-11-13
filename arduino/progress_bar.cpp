/*

Copyright 2024 Ivan Gubochkin (igubochkin@gmail.com)

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be
   used to endorse or promote products derived from this software without specific
   prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <hardwareSerial.h>

#include "progress_bar.h"

// Constructor
ProgressBar::ProgressBar(int total_steps, int bar_width, char fill_char, char empty_char)
    : total_steps(total_steps), bar_width(bar_width), fill_char(fill_char), empty_char(empty_char), last_printed(-1) {}

// Method to initialize the progress bar
void ProgressBar::begin() {
    Serial.println();
    Serial.println(F("Progress:"));
    update(0); // Display initial progress
}

// Method to update the progress bar
void ProgressBar::update(int current_step) {
    // Calculate progress as a percentage
    float progress = (float)current_step / total_steps;
    int percentage = (int)(progress * 100);

    // Update only if the percentage changes
    if (percentage != last_printed) {
        last_printed = percentage;
        print_bar(progress);
    }
}

// Method to display the progress bar
void ProgressBar::print_bar(float progress) {
    int filled_length = (int)(bar_width * progress);
    Serial.print(F("["));
    for (int i = 0; i < bar_width; i++) {
        if (i < filled_length) {
            Serial.print(fill_char);
        } else {
            Serial.print(empty_char);
        }
    }

    Serial.print(F("] "));
    Serial.print((int)(progress * 100));
    Serial.println(F("%"));
}

// Method to finalize the progress bar
void ProgressBar::end() {
    update(total_steps); // Ensure bar is filled to 100%
    Serial.println();
}

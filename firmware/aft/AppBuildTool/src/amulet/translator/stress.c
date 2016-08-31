float WekaClassifier_0_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 1; } else if (i[0] <= 0.970533) { return 1; } else { return 0; }
  }

float WekaClassifier_1_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 0; } else if (i[0] <= 0.9496035) { return 1; } else { return 0; }
  }

float WekaClassifier_2_classify(float i[]) {
    /* fft_coef_0002 */
    if (i[2] == 0.0) { return 0; } else if (i[2] <= 0.11253350000000001) { return 1; } else { return 0; }
  }

float WekaClassifier_3_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 1; } else if (i[0] <= 0.970533) { return 1; } else { return 0; }
  }

float WekaClassifier_4_classify(float i[]) {
    /* fft_coef_0002 */
    if (i[2] == 0.0) { return 0; } else if (i[2] <= 0.1446245) { return 0; } else { return 1; }
  }

float WekaClassifier_5_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 1; } else if (i[0] <= 0.960494) { return 1; } else { return 0; }
  }

float WekaClassifier_6_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 0; } else if (i[0] <= 0.876441) { return 0; } else { return 1; }
  }

float WekaClassifier_7_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 0; } else if (i[0] <= 0.970533) { return 0; } else { return 0; }
  }

float WekaClassifier_8_classify(float i[]) {
    /* fft_coef_0000 */
    if (i[0] == 0.0) { return 0; } else if (i[0] <= 0.970533) { return 1; } else { return 0; }
  }

float WekaClassifier_9_classify(float i[]) {
    /* fft_coef_0001 */
    if (i[1] == 0.0) { return 0; } else if (i[1] <= 0.0199055) { return 1; } else { return 0; }
  }

float min(float a,float b){
    if(a<b)
        return a;
    return b;
}

float computeAverage(float val_array[],int len){
    float rv=0;
    for(int i=0;i<len;i++)
        rv+=val_array[i];
    return rv/(float)len;
}

float computeVar(float val_array[],int len,float average){
    float rv=0;
    for(int i=0;i<len;i++)
        rv+=((val_array[i]-average)*(val_array[i]-average)/(float)len);
    return rv;
}

int main(int argc, char const argv[])
{
  /* code */
  float value=data[data_ptr];
            //DEBUG
            while(value>0.01){
                local_data[local_ptr++]=value;
                data_ptr=(data_ptr+1)%4313;
                value=data[data_ptr];
            }

            //DEBUG


            //UARTprintf("The Interval %d\n", (int)(value*10));
            if(value<0.01){
                                    float pre_average=computeAverage(local_data,local_ptr);
                                    //printf("%s%f\n","pre_average is:", pre_average);
                                    float pre_var=computeVar(local_data,local_ptr,pre_average);
                                    //printf("%s%f\n","pre_var is:", pre_var);
                                    float pre_std_dev=sqrt(pre_var);
                                    //printf("%s%f\n","pre_std_dev is:", pre_std_dev);

                                    int filter_idx=0;
                                    //Filter
                                    for (int i = 0; i < local_ptr; ++i)
                                    {
                                        if(local_data[i]<pre_average+2*pre_std_dev && local_data[i]>pre_average-2*pre_std_dev)
                                            local_data[filter_idx++]=local_data[i];
                                    }

                                    //printf("%s%d\n", "Now data length is ",filter_idx);

                                    float after_average=computeAverage(local_data,filter_idx);
                                    //printf("%s%f\n","after_average is:", after_average);
                                    float after_var=computeVar(local_data,filter_idx,after_average);
                                    //printf("%s%f\n","after_var is:", after_var);
                                    float after_std_dev=sqrt(after_var);
                                    //printf("%s%f\n","after_std_dev is:", after_std_dev);

                                    //printf("%f,%f,%f,", after_average,after_var,after_std_dev);

                                    if(CLASSIFY){
                                        float input[3];
                                        input[0]=after_average;
                                        input[1]=after_var;
                                        input[2]=after_std_dev;
                                        int result=(int)(classify(input));
                                        UARTprintf("Current result is %d and HMM is %d\n",result, HMM );
                                        if(result<0.5 && HMM>-2)
                                                HMM--;
                                        if(result>=0.5 && HMM<5)
                                                HMM++;

                                        if(HMM<=0){
                                               UARTprintf("Not stressed \n" );
                                                CoreLedOff(1, REQUESTS_DEMO_APP);
                                        }
                                        else{
                                               UARTprintf("Stressed \n" );
                                               CoreLedOn(1, REQUESTS_DEMO_APP);
                                        }
                                    }

                local_ptr=0;
            }
            else{
            local_data[local_ptr++]=value;
            }
            data_ptr=(data_ptr+1)%4313;



            status_ = Q_HANDLED();
            break;
  return 0;
}

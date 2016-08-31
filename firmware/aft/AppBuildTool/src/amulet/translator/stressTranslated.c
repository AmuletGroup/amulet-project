typedef struct {
	int *values;
	unsigned int __arr_len;
} __int_array;
typedef struct {
	float *values;
	unsigned int __arr_len;
} __float_array;
typedef struct {
	char *values;
	unsigned int __arr_len;
} __char_array;
/*Have to add further typedefs for other datatypes we use*/
#define getArrayLength(a) a.__arr_len
#define __read_array(a,index) (index >= 0 && index < a.__arr_len)?a.values[index]:-111111111
float WekaClassifier_0_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 1; } else if (__read_array(i,0) <= 0.970533) { return 1; } else { return 0; }
  }


float WekaClassifier_1_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 0; } else if (__read_array(i,0) <= 0.9496035) { return 1; } else { return 0; }
  }


float WekaClassifier_2_classify(__float_array i) {
    
    if (__read_array(i,2) == 0.0) { return 0; } else if (__read_array(i,2) <= 0.11253350000000001) { return 1; } else { return 0; }
  }


float WekaClassifier_3_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 1; } else if (__read_array(i,0) <= 0.970533) { return 1; } else { return 0; }
  }


float WekaClassifier_4_classify(__float_array i) {
    
    if (__read_array(i,2) == 0.0) { return 0; } else if (__read_array(i,2) <= 0.1446245) { return 0; } else { return 1; }
  }


float WekaClassifier_5_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 1; } else if (__read_array(i,0) <= 0.960494) { return 1; } else { return 0; }
  }


float WekaClassifier_6_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 0; } else if (__read_array(i,0) <= 0.876441) { return 0; } else { return 1; }
  }


float WekaClassifier_7_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 0; } else if (__read_array(i,0) <= 0.970533) { return 0; } else { return 0; }
  }


float WekaClassifier_8_classify(__float_array i) {
    
    if (__read_array(i,0) == 0.0) { return 0; } else if (__read_array(i,0) <= 0.970533) { return 1; } else { return 0; }
  }


float WekaClassifier_9_classify(__float_array i) {
    
    if (__read_array(i,1) == 0.0) { return 0; } else if (__read_array(i,1) <= 0.0199055) { return 1; } else { return 0; }
  }


float min(float a,float b){
    if(a<b)
        return a;
    return b;
}


float computeAverage(__float_array val_array,int len){
    float rv=0;
    for(int i=0;i<len;i++)
        rv+=__read_array(val_array,i);
    return rv/(float)len;
}


float computeVar(__float_array val_array,int len,float average){
    float rv=0;
    for(int i=0;i<len;i++)
        rv+=((__read_array(val_array,i)-average)*(__read_array(val_array,i)-average)/(float)len);
    return rv;
}


int main(int argc, __charconst_array argv)
{
  
  float value=__read_array(data,data_ptr);
            
            while(value>0.01){
                if (local_ptr++ >= 0 && local_data.__arr_len > local_ptr++) {
	local_data.values[local_ptr++] = value;
}
                data_ptr=(data_ptr+1)%4313;
                value=__read_array(data,data_ptr);
            }

            


            
            if(value<0.01){
                                    float pre_average=computeAverage(local_data,local_ptr);
                                    
                                    float pre_var=computeVar(local_data,local_ptr,pre_average);
                                    
                                    float pre_std_dev=sqrt(pre_var);
                                    

                                    int filter_idx=0;
                                    
                                    for (int i = 0; i < local_ptr; ++i)
                                    {
                                        if(__read_array(local_data,i)<pre_average+2*pre_std_dev && __read_array(local_data,i)>pre_average-2*pre_std_dev)
                                            if (filter_idx++ >= 0 && local_data.__arr_len > filter_idx++) {
	local_data.values[filter_idx++] = __read_array(local_data,i);
}
                                    }

                                    

                                    float after_average=computeAverage(local_data,filter_idx);
                                    
                                    float after_var=computeVar(local_data,filter_idx,after_average);
                                    
                                    float after_std_dev=sqrt(after_var);
                                    

                                    

                                    if(CLASSIFY){
                                        float __internal_input[3];
__float_array input = { .values = &__internal_input, .__arr_len = 3 };

                                        if (0 >= 0 && input.__arr_len > 0) {
	input.values[0] = after_average;
}
                                        if (1 >= 0 && input.__arr_len > 1) {
	input.values[1] = after_var;
}
                                        if (2 >= 0 && input.__arr_len > 2) {
	input.values[2] = after_std_dev;
}
                                        int result=(int)(classify(input));
                                        UARTprintf("Current result is %d and HMM is %d\n",result, HMM );
                                        if(result<0.5 && HMM>-2)
                                                HMM--;
                                        if(result>=0.5 && HMM<5)
                                                HMM++;

                                        if(HMM<=0){
                                               UARTprintf("Not stressed \n" );
                                                CoreLedOff(1,REQUESTS_DEMO_APP,2);
                                        }
                                        else{
                                               UARTprintf("Stressed \n" );
                                               CoreLedOn(1,REQUESTS_DEMO_APP,2);
                                        }
                                    }

                local_ptr=0;
            }
            else{
            if (local_ptr++ >= 0 && local_data.__arr_len > local_ptr++) {
	local_data.values[local_ptr++] = value;
}
            }
            data_ptr=(data_ptr+1)%4313;



            status_ = Q_HANDLED();
            break;
  return 0;
}



#include<stdio.h>
#include <stdlib.h>
#define K 3 //số K tự chọn 
#define TABLE_SIZE 9973//số nguyên tố an toàn để tránh va trạm trong hash table

typedef struct {
  double dac_trung[10];
  double khoang_cach;
  char label[100];
}data;

void swap(data *a, data *b){
  data t = *a;
  *a = *b;
  *b = t;
}

double l2_norm_0(double *a,int len){
  double l2 = 0;
  for(int i = 0;i < len;i++){
    l2 += a[i]*a[i]; 
  }
  return l2;//ko sqrt vi ko can
}
//nhan hai ma tran kich thuoc 1xd va dx1 nen tra ve 1 ket qua duy nhat
double matrix_multi(double *a,double *b,int d){
  double result = 0;
  for(int i = 0;i<d;i++){
    result += a[i]*b[i];
  }
  return result;
}

//su dung euclidean dist hay chuan l2 de tinh khoang cach
double* khoang_cach_diem(double *point, data data[],int data_n_len,int data_d_len){
  double *khoang_cach = malloc(data_n_len*sizeof(double));
  const double l2_point = l2_norm_0(point, data_d_len);
  double l2_data[data_n_len];
  for(int i = 0;i<data_n_len;i++){
    l2_data[i] = l2_norm_0(data[i].dac_trung,data_d_len);
    //print_info(l2_data, data_n_len);
  }
  
  for(int i = 0;i<data_n_len;i++){
     const double a = matrix_multi(point, data[i].dac_trung, data_d_len);
     khoang_cach[i] = l2_point + l2_data[i] - (a+a);
     //print_info(khoang_cach, data_n_len);
  }
  return khoang_cach;
}
void print_info(data *a,int len,int d){
  puts("label: ");
  for(int i = 0;i<len;i++){
    
    printf("%s ",a[i].label);
  }
  puts("\nkhoang cach: ");
  for(int i = 0;i < len;i++){
    printf("%lf ",a[i].khoang_cach);
  }
  puts("\ndac trung: ");
  const int SIZE = d;
  for(int i = 0;i < len;i++){
    for(int j = 0;j<SIZE;j++){
      printf("%lf ",a[i].dac_trung[j]);
    }
    puts("\n");
  }
}
unsigned int hash(char *a){
  unsigned int hash = 5381;
  int c;
  while((c=*a++)){
    hash = ((hash << 5) + hash) + c;
  }
  return hash % TABLE_SIZE;
}
int main(){
  freopen("data.inp","r",stdin);// dữ liệu cần thiết lấy từ file data.inp. file nay nen duoc xu ly bang python de ra dung dinh dang
  int n=3, d=3;// số lượng mẫu, số đặc chưng
  scanf("%d",&n);
  scanf("%d",&d);
  data training_data[n];
  for(int i = 0;i < n;i++){
    for(int j = 0;j<d;j++){
      scanf("%lf",&training_data[i].dac_trung[j]);
    }
  }
  for(int i = 0;i<n;i++){
    scanf("%s",&training_data[i].label);
  }
  double point[d];
  for(int i = 0;i<d;i++){
    scanf("%lf",&point[i]);
  }
  //print_info(training_data, n,d);
  double *khoang_cach = khoang_cach_diem(point, training_data, n, d);
  for(int i = 0;i<n;i++){
    training_data[i].khoang_cach = khoang_cach[i];
  }
  print_info(training_data, n,d);
  free(khoang_cach);
  khoang_cach = NULL;
  //sort 
  for(int i = 0;i < n - 1;i++){
    int min = i;
    for(int j = i + 1;j<n;j++){
      if(training_data[min].khoang_cach > training_data[j].khoang_cach){
        min = j;
      }
    }
    swap(&training_data[min],&training_data[i]);
  }
  print_info(training_data, n,d);
  
  int hash_table[TABLE_SIZE];
  for(int i = 0;i < TABLE_SIZE;i++){
    hash_table[i] = -1;
  }
  for(int i = 0;i < K;i++){//khoi tao hashmap
    unsigned int index = hash(training_data[i].label);
    if(hash_table[index] == -1){
      hash_table[index] = 1;
    }else{
      hash_table[index]++;
    }
  }
  int max = -9;
  char* max_pointer;
  for(int i = 0;i < K;i++){
    unsigned int index = hash(training_data[i].label);
    if(hash_table[index] > max){
      max = hash_table[index];
      max_pointer = training_data[i].label;
    }
  }
  printf("%s",max_pointer);
}
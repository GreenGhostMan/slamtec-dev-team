အောက်ပါတို့ကိုပြင်ပါ။

HOME_TREE := ../

MAKE_TARGETS := 01Hello 02map2bmp

HOME_TREE ကို slamware_sdk_linux-x86_64-gcc4.8 ရှိရာ path ထည့်ပါ။ 

- build လုပ်ရန်

$ make

- debug လုပ်ဖို့အောက်ပါ command ကို မ run မီ release dir ထဲက include နဲ့ lib ဖိုင်များကို debug dir ထဲ ကူးထည့်ပါ။

$ make DEBUG=1

- build process ကိုကြည့်ချင်ရင်

$ make V=1 

- binary ကို ဖျက်ဖို့

$ make clean

executable များကို /path/to/slamware_sdk_linux-x86_64-gcc4.8/linux-x86_64-release/output တွင်ရယူပါ။ development မြန်အောင်ဒီလိုလုပ်ပါ။

$ runapp="/path/to/slamware_sdk_linux-x86_64-gcc4.8/linux-x86_64-release/output/"
$ make &> out
$ $runapp/your_bin

container ထဲမှာ  vim , gdb တို့ကို install လုပ်ထားပါ။ 

compiler ပြောင်းချင်ပါက mak_def.inc မှာ CC နဲ့ CXX ကိုပြင်ပါ။

support compiler များကတော့ x86_64 ( recommend ubuntu 14.04 ) အတွက် gcc-4.6, g++-4.6, gcc-4.8, g++-4.8 ဖြစ်ပြီး
arm အတွက်ကတော့ arm-linux-gnueabihf-gcc-4.8 နဲ့ arm-linux-gnueabihf-g++-4.8 ဖြစ်ပါတယ်။



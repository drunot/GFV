clear;
clc;
[file,path] = uigetfile('*.wav','�ben fil');
if file ~= 0
encode_wav(file,path);
end
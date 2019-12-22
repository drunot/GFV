clear;
clc;
[file,path] = uigetfile('*.wav','Åben fil');
if file ~= 0
encode_wav(file,path);
end
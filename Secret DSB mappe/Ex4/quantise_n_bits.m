function [x_quant, k] = quantise_n_bits(x_input, n_bits)
% Quantisation of a sequence to n bits, excluding sign bit.
% Using rounding, this quantiser is unbiased.
% Calculation of the step size assumes the input lie in the interval [-1; 1].
% The function wil also work outside this number range.
% Based on quantizeN.m from lgj@ase.au.dk
% 2019-05-07 shn@ase.au.dk

step_size = 1/2^n_bits;

% ***** Forward quantization stage
k = round(x_input/step_size);   % Alt.: floor(), ceil().

% ***** Reconstruction stage
x_quant = step_size*k;


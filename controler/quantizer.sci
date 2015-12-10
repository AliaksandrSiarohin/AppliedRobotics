function out = Quantizer(in)

    // Inputs
    Power = in(1);
    
    // Quantization
    Quanta = 1;
    Power = round(Power/Quanta)*Quanta;
    
    // Output
    out = Power;
    
endfunction

function [x,y,typ] = BRO_Controller(job,arg1,arg2)
    x=[];y=[];typ=[];
    select job
    case 'plot' then
        exprs=arg1.graphics.exprs;

        standard_draw(arg1)
    case 'getinputs' then
        [x,y,typ]=standard_inputs(arg1)
    case 'getoutputs' then
        [x,y,typ]=standard_outputs(arg1)
    case 'getorigin' then
        [x,y]=standard_origin(arg1)
    case 'set' then
        x=arg1
        model=arg1.model;graphics=arg1.graphics;
        exprs=graphics.exprs;
    case 'define' then
        model           = scicos_model();
        model.sim       = list('bro_comm_controller',4);
        model.out       = 1;
        model.outtyp    = 3;
        model.in        = [];
        model.evtin     = [];
        model.rpar      = [];
        model.ipar      = [];
        model.dstate    = [1];
        model.blocktype = 'c';
        model.dep_ut=[%t %f]
        exprs=[]
        gr_i=['xstringb(orig(1),orig(2),[''BRO Controller''],sz(1),sz(2),''fill'');']
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction

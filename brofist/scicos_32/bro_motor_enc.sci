function [x,y,typ] = SPAM_Motor(job,arg1,arg2)
    x=[];y=[];typ=[];
    sensor = '';
    port_no = '';
    select job
        case 'plot' then
            graphics=arg1.graphics;
            ierr=execstr('(evstr(graphics.exprs(1))==1)','errcatch')
            if ierr<>0 then
                sensor = 'Custom Sensor';                
            end
            printf("INIT Sensor: %s, port: %s\n", graphics.exprs(1), graphics.exprs(2));
            select evstr(graphics.exprs(1))
                case 1 then
                    sensor = 'RAW Power'
                case 2 then
                    sensor = 'Degree Per Second'
                else
                    sensor = '?'
            end
            select evstr(graphics.exprs(2))
                case 1 then
                    port_no = 'A'
                case 2 then
                    port_no = 'B'
                case 3 then
                    port_no = 'C'
                else
                    port_no = '?'
            end
            standard_draw(arg1)
        case 'getinputs' then
            [x,y,typ]=standard_inputs(arg1)
        case 'getoutputs' then
            [x,y,typ]=standard_outputs(arg1)
        case 'getorigin' then
            [x,y]=standard_origin(arg1)
        case 'set' then
            x = arg1;
            graphics = arg1.graphics;
            exprs = graphics.exprs;
            model = arg1.model;
            while %t do
                [ok,sensor_typ,port,exprs]=getvalue('Set Sensors Parameters',..
                    ['Data Type (1 for RAW Power, 2 for DegPS)';'Port (1~3 for A->C)'],..
                    list('vec',1,'vec',1),exprs)
                if ~ok then
                    break
                end
                sensor_typ = int(sensor_typ);
                port = int(port);
                printf("Sensor: %d, port: %d\n", sensor_typ, port);
                if (((sensor_typ < 1) | (sensor_typ > 2)) | ((port < 1) | (port > 3)) ) then
                    message('Wrong data! Data Type should be either 1 or 2 and port should be a number between 1 and 3');
                else
                    printf("Post if: %d, port: %d\n", sensor_typ, port);
                    graphics.exprs = exprs; 
                    model.dstate = 0;
                    model.ipar = [sensor_typ; port];
                    x.graphics = graphics;
                    x.model = model;
                    break
                end
            end
        case 'define' then
            sensor_typ = 1;
            port = 1;
            model=scicos_model()
            model.sim=list('bro_motor_enc',4)
            model.out = [3];
            model.out2 = [1];
            model.outtyp = [1];
            model.in =    [1];
            model.in2 =   [1];
            model.intyp = [1];
            model.evtin=[]
            model.rpar=[]
            model.ipar = [sensor_typ; port];
            model.dstate=[1];
            model.blocktype='c'
            model.dep_ut=[%t %f]
            exprs = [string(sensor_typ);string(port)];
            gr_i=['xstringb(orig(1),orig(2),[sensor+'' on Port ''+port_no],sz(1),sz(2),''fill'');']
            x=standard_define([3 2],model,exprs,gr_i)
        end
endfunction

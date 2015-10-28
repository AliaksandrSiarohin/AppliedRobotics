base = "/home/ubuntu/NXT/brofist/scicos_32/";

link(base+'./libbro_comm_blocks.so',['bro_comm_sens_disp','bro_comm_controller','bro_comm_motor_disp'],'c');
link(base+'./libbro_sensor_enc.so',['bro_sensor_enc'],'c');
link(base+'./libbro_motor_enc.so',['bro_motor_enc'],'c');

genlib('lib_lego',pwd());

exec bro_sensor_enc.sci;
exec bro_comm_controller.sci;
exec bro_comm_sens_disp.sci;
exec bro_comm_motor_disp.sci;
exec bro_motor_enc.sci;

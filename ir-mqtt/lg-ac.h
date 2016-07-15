void ac_send_code(unsigned long code, IRsend *irsend);
void ac_activate(int temperature, int air_flow, IRsend *irsend);
void ac_change_air_swing(int air_swing, IRsend *irsend);
void ac_power_down(IRsend *irsend);
void ac_air_clean(int air_clean, IRsend *irsend);
import hal
import gi

from gi.repository import Gtk, GObject


def get_hal_name(comp_id, pe_id, suffix):
    return f"pokeys.{comp_id}.PEv2.{pe_id}.{suffix}"


class PoKeysAxisWidget(Gtk.Box):
    __gtype_name__ = 'PoKeysAxisWidget'

    def __init__(self, comp_id=0, axis_id=0):
        super().__init__(orientation=Gtk.Orientation.VERTICAL, spacing=6)

        self.comp_id = comp_id
        self.axis_id = axis_id

        self.pos_max_param = None
        self.cur_pos_pin = None
        self.ref_pos_pin = None

        self.scale = Gtk.Scale.new_with_range(Gtk.Orientation.HORIZONTAL, -100000, 100000, 1)
        self.scale.set_digits(0)
        self.scale.set_sensitive(False)
        self.pack_start(self.scale, True, True, 0)

        self.label = Gtk.Label(label="Position: 0")
        self.pack_start(self.label, False, False, 0)

        self.reconnect()

        # Timer to update values
        GObject.timeout_add(100, self.update_display)  # 100ms

    def reconnect(self):
        self.pos_max_param = get_hal_name(self.comp_id, self.axis_id, "digin.SoftLimit.PosMax")
        self.cur_pos_pin = get_hal_name(self.comp_id, self.axis_id, "CurrentPosition")
        self.ref_pos_pin = get_hal_name(self.comp_id, self.axis_id, "ReferencePosition")

    def update_display(self):
        try:
            cur_pos = hal.get_value(self.cur_pos_pin)
            self.scale.set_value(cur_pos)
            self.label.set_text(f"Current: {cur_pos} steps")
        except Exception as e:
            self.label.set_text(f"HAL error: {e}")
        return True  # Keep the timer running

    def set_softlimit(self, value):
        try:
            hal.set_param(self.pos_max_param, value)
        except Exception as e:
            print(f"Failed to set PosMax: {e}")

    def set_axis_id(self, new_axis_id):
        self.axis_id = new_axis_id
        self.reconnect()

    def set_comp_id(self, new_comp_id):
        self.comp_id = new_comp_id
        self.reconnect()

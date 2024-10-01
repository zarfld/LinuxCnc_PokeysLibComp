class Settings:
    def __init__(self, telemetry_opt_in=False):
        self.telemetry_opt_in = telemetry_opt_in

    def set_telemetry_opt_in(self, opt_in):
        self.telemetry_opt_in = opt_in

    def get_telemetry_opt_in(self):
        return self.telemetry_opt_in

    def save_settings(self, file_path):
        with open(file_path, 'w') as file:
            file.write(f"telemetry_opt_in={self.telemetry_opt_in}\n")

    def load_settings(self, file_path):
        try:
            with open(file_path, 'r') as file:
                for line in file:
                    if line.startswith("telemetry_opt_in="):
                        self.telemetry_opt_in = line.split('=')[1].strip().lower() == 'true'
        except FileNotFoundError:
            pass

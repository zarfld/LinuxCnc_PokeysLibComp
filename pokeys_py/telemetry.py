import sentry_sdk

class Telemetry:
    def __init__(self, dsn, opt_in=False):
        self.dsn = dsn
        self.opt_in = opt_in
        self.sentry_initialized = False

    def initialize_sentry(self):
        if self.opt_in and not self.sentry_initialized:
            sentry_sdk.init(self.dsn)
            self.sentry_initialized = True

    def track_event(self, event_name, event_data=None):
        if self.opt_in and self.sentry_initialized:
            with sentry_sdk.configure_scope() as scope:
                if event_data:
                    for key, value in event_data.items():
                        scope.set_extra(key, value)
                sentry_sdk.capture_message(event_name)

    def track_exception(self, exception):
        if self.opt_in and self.sentry_initialized:
            sentry_sdk.capture_exception(exception)

    def track_performance(self, transaction_name, transaction_data=None):
        if self.opt_in and self.sentry_initialized:
            with sentry_sdk.start_transaction(name=transaction_name) as transaction:
                if transaction_data:
                    for key, value in transaction_data.items():
                        transaction.set_tag(key, value)
                transaction.finish()

    def track_ui_setup_tool_event(self, event_name, event_data=None):
        self.track_event(event_name, event_data)

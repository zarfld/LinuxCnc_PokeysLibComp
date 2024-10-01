import os
import sentry_sdk

def init_telemetry():
    if os.getenv("TELEMETRY_ENABLED") == "true":
        sentry_sdk.init(
            dsn=os.getenv("SENTRY_DSN"),
            traces_sample_rate=1.0
        )

def capture_exception(exception):
    if os.getenv("TELEMETRY_ENABLED") == "true":
        sentry_sdk.capture_exception(exception)

def capture_message(message):
    if os.getenv("TELEMETRY_ENABLED") == "true":
        sentry_sdk.capture_message(message)

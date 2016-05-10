export const BOT_STARTING = "BOT_STARTING";
export const BOT_RUNNING = "BOT_RUNNING";
export const BOT_STOPPING = "BOT_STOPPING";
export const BOT_STOPPED = "BOT_STOPPED";

export function start() {
    return {
        type: BOT_STARTING
    };
}

export function started() {
    return {
        type: BOT_RUNNING
    };
}

export function stop() {
    return {
        type: BOT_STOPPING
    }
}

export function stopped() {
    return {
        type: BOT_STOPPED
    }
}

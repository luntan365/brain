import {
    BOT_ERROR,
    BOT_STARTING,
    BOT_RUNNING,
    BOT_STOPPING,
    BOT_STOPPED
} from '../actions/bot';

export default function bot(state = "Idle", action) {
  switch (action.type) {
    case BOT_STARTING:
      return "Starting";
    case BOT_RUNNING:
      return "Running";
    case BOT_STOPPING:
      return "Stopping";
    case BOT_ERROR:
      return "Error";
    case BOT_STOPPED:
    default:
      return "Idle";
  }
}

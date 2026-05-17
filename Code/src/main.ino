String take_debug_call(int errorcode) {
  switch (errorcode) {

    case 1:
      return "Pokemon system not initialized";

    case 2:
      return "Null pointer / invalid object reference";

    case 3:
      return "Attack execution failed";

    case 4:
      return "Not enough energy to perform action";

    case 5:
      return "Invalid type matchup calculation";

    case 6:
      return "Index out of range (array/vector overflow)";

    case 7:
      return "Memory allocation failed (heap full)";

    case 8:
      return "Battle state corrupted";

    case 9:
      return "Turn order desync detected";

    case 10:
      return "Pokemon fainted but still active in queue";

    case 11:
      return "Invalid move selected";

    case 12:
      return "Move cooldown not finished";

    case 13:
      return "Critical hit calculation failed";

    case 14:
      return "Damage calculation overflow";

    case 15:
      return "Status effect application failed";

    case 16:
      return "Unknown Pokemon type";

    case 17:
      return "Type effectiveness table missing entry";

    case 18:
      return "Invalid type conversion";

    case 19:
      return "Type matchup matrix corrupted";

    case 20:
      return "Resistance calculation failed";

    case 21:
      return "AI decision tree crashed";

    case 22:
      return "No valid move found for AI";

    case 23:
      return "AI stuck in infinite loop";

    case 24:
      return "Random generator failure";

    case 25:
      return "Behavior model not loaded";

    case 26:
      return "Controller input missing";

    case 27:
      return "Button debounce failure";

    case 28:
      return "Invalid menu selection";

    case 29:
      return "Input buffer overflow";

    case 30:
      return "Command parsing failed";

    case 31:
      return "Sprite not found";

    case 32:
      return "LCD display init failed";

    case 33:
      return "UI rendering overflow";

    case 34:
      return "Screen refresh timeout";

    case 35:
      return "Graphics buffer corrupted";

    case 36:
      return "Save file missing";

    case 37:
      return "Save file corrupted";

    case 38:
      return "Write permission denied";

    case 39:
      return "Load operation failed";

    case 40:
      return "Data checksum mismatch";

    case 41:
      return "Connection timeout";

    case 42:
      return "data loss detected";

    case 43:
      return "Sync failed with game";

    case 44:
      return "Invalid handshake between devices";

    case 45:
      return "Disconnected unexpectedly";

    case 46:
      return "Subsystem failure: math engine";

    case 47:
      return "Subsystem failure: animation engine";

    case 48:
      return "Subsystem failure: event handler";

    case 49:
      return "Subsystem failure: scheduler";

    case 50:
      return "Subsystem failure: timer interrupt";

    case 51: case 52: case 53: case 54: case 55:
      return "Runtime instability detected";

    case 56: case 57: case 58: case 59: case 60:
      return "Unexpected state transition";

    case 61: case 62: case 63: case 64: case 65:
      return "Resource lock failure";

    case 66: case 67: case 68: case 69: case 70:
      return "Data synchronization error";

    case 71: case 72: case 73: case 74: case 75:
      return "Execution pipeline stalled";

    case 76: case 77: case 78: case 79: case 80:
      return "Subsystem overload detected";

    case 81: case 82: case 83: case 84: case 85:
    case 86: case 87: case 88: case 89: case 90:
      return "Critical system warning: instability increasing";

    case 91: case 92: case 93: case 94: case 95:
    case 96: case 97: case 98: case 99: case 100:
      return "System integrity compromised";

    case 101: case 102: case 103: case 104: case 105:
    case 106: case 107: case 108: case 109: case 110:
      return "Emergency fallback mode triggered";

    case 111: case 112: case 113: case 114: case 115:
    case 116: case 117: case 118: case 119: case 120:
      return "Core logic error detected";

    case 121: case 122: case 123: case 124: case 125:
    case 126: case 127: case 128: case 129: case 130:
      return "Unhandled exception in battle engine";

    case 131: case 132: case 133: case 134: case 135:
    case 136: case 137: case 138: case 139: case 140:
      return "State machine desynchronization";

    case 141: case 142: case 143: case 144: case 145:
    case 146: case 147: case 148: case 149: case 150:
      return "Memory corruption suspected";

    case 151: case 152: case 153: case 154: case 155:
    case 156: case 157: case 158: case 159: case 160:
      return "Unexpected null dereference";

    case 161: case 162: case 163: case 164: case 165:
    case 166: case 167: case 168: case 169: case 170:
      return "Input pipeline failure";

    case 171: case 172: case 173: case 174: case 175:
    case 176: case 177: case 178: case 179: case 180:
      return "AI subsystem instability";

    case 181: case 182: case 183: case 184: case 185:
    case 186: case 187: case 188: case 189: case 190:
      return "Graphics subsystem desync";

    case 191: case 192: case 193: case 194: case 195:
    case 196: case 197: case 198: case 199: case 200:
      return "System shutdown";

    default:
      return "Unknown error code";
  }
}
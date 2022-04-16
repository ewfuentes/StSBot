
# Slay The Spire Bot

## Initial Setup
1. Install system dependencies:
```
  # apt install libzmq3-dev libzmqpp-dev
```
2. Build the bridge and the bot (bazel build //interface/...)
3. Install Slay the Spire through Steam
4. Through Steam work bench, add the following mods:
 - BaseMod
 - StsLib
 - CommunicationMod
5. Run the Slay the Spire with mods enabled once and exit.
6. Edit the communication mod config at `~/.config/ModTheSpire/CommunicationMod/config.properties` to contain the following:
```
command=<command to run bridge>
runAtGameStart=true
```
7. Restart Slay the Spire with mods enabled.
8. You should now be able to run the bot with: `<command to run bot>`
9. If you make changes to the bot, you should be able to just restart the bot.

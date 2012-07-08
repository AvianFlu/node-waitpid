
var binding = require('../build/Release/waitpid');

module.exports = function waitpid(pid) {
  //returns the exit code/signal
  return binding.waitpid(pid);
};

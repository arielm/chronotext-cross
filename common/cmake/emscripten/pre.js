var Module = {
  preRun: function() {
    FS.mkdir("/res");
    FS.mount(NODEFS, { root: "../../res" }, "/res");
    console.log('preRun executed');
  },
  postRun: function() {
    FS.unmount("/res");
    console.log('postRun executed');
  },
};

console.log('pre.js executed');

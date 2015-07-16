
enable_testing() # XXX: DOES NOT WORK WHEN INVOKED INSIDE FUNCTION

function(setupTesting)
  if (CONFIG_RUN AND CONFIG_INSTALL)
    configure_file(${CONFIG_RUN} run.sh)
    configure_file(${CONFIG_INSTALL} install.sh)

    install(CODE "
      execute_process(COMMAND ./install.sh RESULT_VARIABLE result)
      if (result)
        message(FATAL_ERROR)
      endif()
    ")
  endif()
endfunction()

# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/userAlg/SConscript,v 1.4 2009/09/12 16:23:13 lsrea Exp $ 
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: userAlg-06-06-04
import os
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

progEnv.Tool('FluxSvcLib')
progEnv.Tool('identsLib')
progEnv.Tool('EventLib')
progEnv.Tool('facilitiesLib')
progEnv.Tool('GuiSvcLib')
progEnv.Tool('guiLib')
progEnv.Tool('astroLib')
#progEnv.Tool('GleamLib')
progEnv.Tool('ntupleWriterSvcLib')

if baseEnv['PLATFORM'] != 'win32':
        progEnv.AppendUnique(LINKFLAGS = ['-u GuiSvc_loadRef'])

if baseEnv['PLATFORM'] == 'win32':
        progEnv.AppendUnique(LINKFLAGS =['/include:_GuiSvc_loadRef'])
        progEnv.AppendUnique(LINKFLAGS=['/subsystem:windows'])  #from macro guiapp_linkopts in ../gui/cmt/requirements


userApp = progEnv.GaudiProgram('userApp', listFiles(['src/*.cxx']), test=1)
test_userAlg = progEnv.GaudiProgram('test_userAlg', ['src/UserAlg.cxx', 'src/RegisterSource.cxx'], test = 1)

progEnv.Tool('registerObjects', package = 'userAlg', testApps = [test_userAlg,userApp])





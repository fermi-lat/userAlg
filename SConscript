# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/userAlg/SConscript,v 1.12 2011/12/12 20:59:30 heather Exp $ 
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: userAlg-06-07-00
import os
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
##libEnv = baseEnv.Clone()

progEnv.Tool('FluxSvcLib')
progEnv.Tool('identsLib')
progEnv.Tool('EventLib')
progEnv.Tool('facilitiesLib')
progEnv.Tool('GuiSvcLib')
progEnv.Tool('guiLib')
progEnv.Tool('astroLib')

progEnv.Tool('ntupleWriterSvcLib')

userApp = progEnv.GaudiProgram('userApp', 
			       listFiles(['src/*.cxx']), test=1,
			       package='userAlg')
test_userAlg = progEnv.GaudiProgram('test_userAlg', 
				    ['src/UserAlg.cxx', 
				     'src/RegisterSource.cxx'], 
				    test = 1, package='userAlg')

progEnv.Tool('registerTargets', package = 'userAlg',
	     testAppCxts = [[test_userAlg, progEnv], 
			    [userApp,progEnv]],
	     jo = listFiles(['src/*.txt', 
			     'src/test/jobOptions.txt']),
         xml = listFiles(['xml/*.xml']))





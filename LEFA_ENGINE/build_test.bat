@echo off 

set CC=wcc386
set EXIT=../bin/test_demo/obj

rem set COMPILATION_ARGS= -bm /6r -fp6 -ms -om -op -ot -oz -zkl -zdp -zi -ot  -ol -oo -e600
set COMPILATION_ARGS= /6r -fp6 -e600 -i="lefa/include"

cd SRC
echo.
echo.
echo ======== MAIN.c
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "test_demo/main.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo.
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "test_demo/camera/camera.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo.
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "test_demo/util/util.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo.
echo.
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "lefa/c_src/platforms/windows/windowSystem.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "lefa/c_src/platforms/windows/input_manager.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo.
echo.
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "lefa/c_src/render/api_manager/gl_manager/gl_manager.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo  ======LODEPNG
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "lefa/c_src/3rdparty/lodepng/lodepng.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.
echo GLAD_OPENGL COMPILATIOn
echo.
echo.
@wcc386 %COMPILATION_ARGS% -i="lefa/include" "lefa/c_src/3rdparty/glad/OpenGL/glad_gl.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.


rem DDS_TEXTURE

echo.
@wcc386 /6r -fp6  -e500 -zk -i="lefa/include" "test_demo/dds_texture_test/image_dds.c" -fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo.
echo.


REM ____________________________________________________________________
echo.
echo.
echo.
echo  ___________MODEL.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_ANIMATION.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_animation.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_BONE.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_bone.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_BUILDER.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_builder.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_BUILDER_LOD.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_builder_lod.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_FACES.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_faces.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_HAIR.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_hair.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_LIGHT.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_light.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_LOD.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_lod.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_MANAGER.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_manager.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_MATERIAL.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_material.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_NODE.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_node.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_NODES.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_nodes.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_PARTICLE.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_particle.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_PARTICLE_SYSTEM.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_particle_system.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_PARTITION.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_partition.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_POSE.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_pose.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_POSE_BUFFER.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_pose_buffer.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_POSE_CHANNEL.C_____________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_pose_channel.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_POSE_SKELETON.C____________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_pose_skeleton.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_SHAPE.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_shape.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_SHAPE_KEY.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_shape_key.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_TEXTURE.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_texture.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ___________MODEL_WEIGHT_GROUP.C_____________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/model_weight_group.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
ECHO (________ARCHIVE________)
echo.
echo.
echo.
echo.
echo.
echo  ________________archive_packet.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/archive/archive_packet.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________archive_reader.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/archive/archive_reader.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________archive_tar.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/archive/archive_tar.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________archive_writer.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/archive/archive_writer.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
ECHO (_______SYSTEM________)
echo.
echo.
echo.
echo.
echo  ________________system.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system.c" -Fo="%EXIT%/system__" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_async_call.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_async_call.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_directory.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_directory.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_endian.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_endian.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_error.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_error.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_filesystem.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_filesystem.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_memory.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_memory.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_misc.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_misc.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_mmap.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_mmap.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________system_module.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_module.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_mutex.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_mutex.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_notify.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_notify.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_path.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_path.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________system_paths.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_paths.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_relative.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_relative.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_semaphore.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_semaphore.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_serial_worker.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_serial_worker.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________system_string.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_string.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________system_thread.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_thread.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_time.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_time.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_types.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_types.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_user.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_user.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________system_utf8.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/system/system_utf8.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
ECHO (____algorithm____)
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_array.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_array.c" -Fo="%EXIT%/algorithm_array_" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_astar.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_astar.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_bst.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_bst.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_list.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_list.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_memdic.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_memdic.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_octree.c________________________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_octree.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_priority_queue.c__________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_priority_queue.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_ptrdic.c__________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_ptrdic.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_sectors.c__________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_sectors.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_strdic.c__________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_strdic.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.
echo.
echo.
echo.
echo.
echo.
echo  ________________algorithm_u32dic.c__________________
echo /____________________________________________________________\
     @%cc% %COMPILATION_ARGS% "test_demo/model_test/ANOTHER/algorithm/algorithm_u32dic.c" -Fo="%EXIT%/" ^
     2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"
echo ______________________________________________________________
echo \____________________________________________________________/
echo.


     cd ..
echo.
echo.
@wlink name test_demo.exe ^
          file bin/test_demo/obj/main.obj, ^
                bin/test_demo/obj/camera.obj, ^
                bin/test_demo/obj/util.obj, ^
                bin/test_demo/obj/windowSystem.obj, ^
                bin/test_demo/obj/input_manager.obj, ^
                bin/test_demo/obj/gl_manager.obj, ^
             bin/test_demo/obj/lodepng.obj, ^
                bin/test_demo/obj/glad_gl.obj, ^
                bin/test_demo/obj/algorithm_array_.obj, ^
                bin/test_demo/obj/algorithm_astar.obj, ^
                bin/test_demo/obj/algorithm_bst.obj, ^
                bin/test_demo/obj/algorithm_list.obj, ^
                bin/test_demo/obj/algorithm_memdic.obj, ^
                bin/test_demo/obj/algorithm_octree.obj, ^
                bin/test_demo/obj/algorithm_priority_queue.obj, ^
                bin/test_demo/obj/algorithm_ptrdic.obj, ^
                bin/test_demo/obj/algorithm_sectors.obj, ^
                bin/test_demo/obj/algorithm_strdic.obj, ^
                bin/test_demo/obj/algorithm_u32dic.obj, ^
                bin/test_demo/obj/archive_packet.obj, ^
                bin/test_demo/obj/archive_reader.obj, ^
                bin/test_demo/obj/archive_tar.obj, ^
                bin/test_demo/obj/archive_writer.obj, ^
                bin/test_demo/obj/image_dds.obj, ^
                bin/test_demo/obj/model.obj, ^
                bin/test_demo/obj/model_animation.obj, ^
                bin/test_demo/obj/model_bone.obj, ^
                bin/test_demo/obj/model_builder.obj, ^
                bin/test_demo/obj/model_builder_lod.obj, ^
                bin/test_demo/obj/model_faces.obj, ^
                bin/test_demo/obj/model_hair.obj, ^
                bin/test_demo/obj/model_light.obj, ^
                bin/test_demo/obj/model_lod.obj, ^
                bin/test_demo/obj/model_manager.obj, ^
                bin/test_demo/obj/model_material.obj, ^
                bin/test_demo/obj/model_node.obj, ^
                bin/test_demo/obj/model_nodes.obj, ^
                bin/test_demo/obj/model_particle.obj, ^
                bin/test_demo/obj/model_particle_system.obj, ^
                bin/test_demo/obj/model_partition.obj, ^
                bin/test_demo/obj/model_pose.obj, ^
                bin/test_demo/obj/model_pose_buffer.obj, ^
                bin/test_demo/obj/model_pose_channel.obj, ^
                bin/test_demo/obj/model_pose_skeleton.obj, ^
                bin/test_demo/obj/model_shape.obj, ^
                bin/test_demo/obj/model_shape_key.obj, ^
                bin/test_demo/obj/model_texture.obj, ^
                bin/test_demo/obj/model_weight_group.obj, ^
                bin/test_demo/obj/system__.obj, ^
                bin/test_demo/obj/system_async_call.obj, ^
                bin/test_demo/obj/system_directory.obj, ^
                bin/test_demo/obj/system_endian.obj, ^
                bin/test_demo/obj/system_error.obj, ^
                bin/test_demo/obj/system_filesystem.obj, ^
                bin/test_demo/obj/system_memory.obj, ^
                bin/test_demo/obj/system_misc.obj, ^
                bin/test_demo/obj/system_mmap.obj, ^
                bin/test_demo/obj/system_module.obj, ^
                bin/test_demo/obj/system_mutex.obj, ^
                bin/test_demo/obj/system_notify.obj, ^
                bin/test_demo/obj/system_path.obj, ^
                bin/test_demo/obj/system_paths.obj, ^
                bin/test_demo/obj/system_relative.obj, ^
                bin/test_demo/obj/system_semaphore.obj, ^
                bin/test_demo/obj/system_serial_worker.obj, ^
                bin/test_demo/obj/system_string.obj, ^
                bin/test_demo/obj/system_thread.obj, ^
                bin/test_demo/obj/system_time.obj, ^
                bin/test_demo/obj/system_types.obj, ^
                bin/test_demo/obj/system_user.obj, ^
                bin/test_demo/obj/system_utf8.obj ^
          Library opengl32.lib, ^
               lib/dinput.lib, ^
               lib/dinput8.lib, ^
               lib/dxguid.lib, ^
               lib/pthreadWC2.lib, ^
               lib/zlib.lib, ^
               lib/libiconv.lib
echo.
echo.


move test_demo.exe bin/test_demo/












pause 
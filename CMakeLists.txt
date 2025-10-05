# This is only for Android NDK. Use configure-make for PC.

cmake_minimum_required(VERSION 3.5)

project("gnugo3")

add_library(engine3
engine/aftermath.c
engine/board.c
engine/boardlib.c
engine/breakin.c
engine/cache.c
engine/clock.c
engine/combination.c
engine/dragon.c
engine/endgame.c
engine/filllib.c
engine/fuseki.c
engine/genmove.c
engine/globals.c
engine/handicap.c
engine/hash.c
engine/influence.c
engine/interface.c
engine/matchpat.c
engine/montecarlo.c
engine/move_reasons.c
engine/movelist.c
engine/optics.c
engine/oracle.c
engine/owl.c
engine/persistent.c
engine/printutils.c
engine/readconnect.c
engine/reading.c
engine/semeai.c
engine/sgfdecide.c
engine/sgffile.c
engine/shapes.c
engine/showbord.c
engine/surround.c
engine/unconditional.c
engine/utils.c
engine/value_moves.c
engine/worm.c
)

target_compile_definitions(engine3 PRIVATE HAVE_CONFIG_H)
target_include_directories(engine3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/android
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
    ${CMAKE_CURRENT_SOURCE_DIR}/patterns
    ${CMAKE_CURRENT_SOURCE_DIR}/sgf
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

add_library(patterns3
patterns/aa_attackpat.c
patterns/apatterns.c
patterns/barriers.c
patterns/conn.c
patterns/connections.c
# patterns/dfa.c
patterns/dpatterns.c
patterns/eyes.c
patterns/fuseki9.c
patterns/fuseki13.c
patterns/fuseki19.c
patterns/fusekipat.c
patterns/handipat.c
patterns/helpers.c
patterns/influence.c
patterns/josekidb.c
patterns/mcpat.c
patterns/oraclepat.c
patterns/owl_attackpat.c
patterns/owl_defendpat.c
patterns/owl_vital_apat.c
patterns/endgame.c
patterns/patterns.c
patterns/transform.c
)
target_compile_definitions(patterns3 PRIVATE HAVE_CONFIG_H)
target_include_directories(patterns3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/android
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/sgf
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

add_library(sgf3 
sgf/sgf_utils.c
sgf/sgfnode.c
sgf/sgftree.c
)
target_compile_definitions(sgf3 PRIVATE HAVE_CONFIG_H)
target_include_directories(sgf3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/android
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)


add_library(gnugo3 SHARED
 utils/random.c
 utils/gg_utils.c
 android/dummy.c
 android/jni_bridge.c)
target_compile_definitions(gnugo3 PRIVATE HAVE_CONFIG_H)
target_include_directories(gnugo3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/android
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/sgf
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
)


target_link_libraries(gnugo3
                      engine3
                      patterns3
                      sgf3
                      android
                      log)
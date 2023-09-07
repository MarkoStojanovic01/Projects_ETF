import express from 'express';
import { UserController } from '../controllers/user.controller';
const userRouter = express.Router();

userRouter.route('/login').post(
    (req, res) => new UserController().login(req, res)
)

userRouter.route('/getUser').post(
    (req, res) => new UserController().getUser(req, res)
)

userRouter.route('/getAllDoctors').get(
    (req, res) => new UserController().getAllDoctors(req, res)
)

userRouter.route('/getAllUsers').get(
    (req, res) => new UserController().getAllUsers(req, res)
)

userRouter.route('/deleteUser').post(
    (req, res) => new UserController().deleteUser(req, res)
)

userRouter.route('/addUser').post(
    (req, res) => new UserController().addUser(req, res)
)

userRouter.route('/updatePatient').post(
    (req, res) => new UserController().updatePatient(req, res)
)

userRouter.route('/addImage').post(
    (req, res) => new UserController().addImage(req, res)
)



export default userRouter;